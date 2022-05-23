#include "G_Nemesis.h"

#include "Kismet/KismetMathLibrary.h"

#include "G_NemesisBrain.h"
#include "Greed/UI/HUD/G_HUD.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"
#include "Greed/AI/Nemesis/NemesisDestruct/G_NemesisDestruct.h"

#include "Greed/Manager/Achievements/G_AchievementManager.h"
#include "Greed/Manager/GameInstance/G_GameInstance.h"

AG_Nemesis::AG_Nemesis()
{
	mBrain = CreateDefaultSubobject<UG_NemesisBrain>("Nemesis Brain");
	AddOwnedComponent(mBrain);

	CreatePivotsNecks("Neck Front Head");
	CreatePivotsNecks("Neck Back Left");
	CreatePivotsNecks("Neck Back Right");
}


void AG_Nemesis::CreatePivotsNecks(const FName& _namePivot) 
{
	USceneComponent* _neck = CreateDefaultSubobject<UStaticMeshComponent>(_namePivot);
	_neck->SetupAttachment(mEntityMesh ? mEntityMesh : RootComponent);
	mNecksToRotate.Add(_neck);
}

void AG_Nemesis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (size_t i = 0; i < mNecksToRotate.Num(); i++)
	{
		LookAtPlayerPos(i);
	}
}

void AG_Nemesis::Activate() 
{
	FDataNemesis* _data = GetDataFromTable<FDataNemesis>();
	if (!_data) return;
	InitStatsFromDatatable(*_data);
	//InitComponentsWithData(*_data);
	Super::InitMob();

	for (size_t i = 0; i < mNecksToRotate.Num(); i++)
	{
		USceneComponent* _comp = mNecksToRotate[i];
		if (!_comp) continue;
		mNecksBasicsRotations.Add(_comp->GetComponentRotation());
		mNecksBasicsForwards.Add(_comp->GetForwardVector());
	}
	GetPlayerRef();
	InitChangeTargetDelegate();
}

void AG_Nemesis::InitMob()
{
	if (!mAutoActivate) return;
	FDataNemesis* _data = GetDataFromTable<FDataNemesis>();
	if (!_data) return;
	InitStatsFromDatatable(*_data);
	//InitComponentsWithData(*_data);
	Super::InitMob();

	for (size_t i = 0; i < mNecksToRotate.Num(); i++)
	{
		USceneComponent* _comp = mNecksToRotate[i];
		if (!_comp) continue;
		mNecksBasicsRotations.Add(_comp->GetComponentRotation());
		mNecksBasicsForwards.Add(_comp->GetForwardVector());
	}
	GetPlayerRef();
	InitChangeTargetDelegate();
}

void AG_Nemesis::InitUI() 
{
	UWorld* _world = GetWorld();
	if (!_world) return;
	APlayerController* _controller = _world->GetFirstPlayerController();
	if (!_controller) return;
	AG_HUD* _hud = _controller->GetHUD<AG_HUD>();
	if (!_hud) return;
	_hud->AddNemesisLife(*this);
	mOnDie += [_hud]() {_hud->RemoveNemesisLife(); };
}

void AG_Nemesis::InitStatsFromDatatable(FG_EnemyData& _data)
{
	Super::InitStatsFromDatatable(_data);
	FDataNemesis _dataNemesis = *StaticCast<FDataNemesis*>(&_data);
	mRotationSpeed = _dataNemesis.GetRotationSpeed();
	mMaxAngleTargetBasicForward = _dataNemesis.GetMaxAngleTargetBasicForward();
	mAllPhases = _dataNemesis.GetAllPhases();
	mCurrentPhase = GetPhase(mCurrentPhaseNum);

	FTimerManager& _timeManager = mWorld ? mWorld->GetTimerManager() : GetWorld()->GetTimerManager();
	FTimerHandle _timerHandle = FTimerHandle();
	FTimerDelegate _delegate = FTimerDelegate();
	_delegate.BindLambda([=]
	{
		InitHPDueToPlayerWeapon(_dataNemesis);
		InitUI();
	});
	_timeManager.SetTimer(_timerHandle, _delegate, .1f, false);
	//InitHPDueToPlayerWeapon(_dataNemesis);

	if (!mBrain) return;
	UG_NemesisBrain* _nemesisBrain = StaticCast<UG_NemesisBrain*>(mBrain);
	_nemesisBrain->FillSkillPhase(mCurrentPhase);
}

void AG_Nemesis::InitHPDueToPlayerWeapon(const FDataNemesis& _dataNemesis)
{
	if (!_dataNemesis.UseLifePerWeapon())return;

	APlayerController* _controller = mWorld ? mWorld->GetFirstPlayerController() : GetWorld()->GetFirstPlayerController();
	if (!_controller)return;

	const AG_MainCharacter* _chara = _controller->GetPawn<AG_MainCharacter>();
	if (!_chara) return;

	const UG_Weapon* _weapon = _chara->GetWeapon();
	if (!_weapon) return;

	const ETypeWeapon _type = _weapon->GetTypeWeapon();
	mStatsTarget.MaxLife = mStatsTarget.Life = _dataNemesis.GetLifePerWeapon(_type);
}

void AG_Nemesis::InitChangeTargetDelegate()
{
	mOnChangeTarget.AddLambda([this](ITarget* _target)
	{
		mTargetToLook.SetInterface(_target);
	});

	mOnChangePhase += ([this](const uint8& _newPhase) 
	{
		
	});

	mOnDie += [this]()
	{
		EndSkills();
		SpawnDestructibleNemesis();
	};

	mOnDie += [this]()
	{
		EndSkills();
		SpawnDestructibleNemesis();
	};
}


void AG_Nemesis::GetPlayerRef()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller) return;
	ITarget* _target = Cast<ITarget>(_controller->GetPawn());
	if (!_target) return;
	mTargetToLook.SetInterface(_target);
	mTargetToLook.SetObject(_controller->GetPawn());
}

void AG_Nemesis::ClearAction()
{
	Super::ClearAction();
	mOnActivateBoss.Clear();
	mOnChangePhase.Clear();
	mOnEndDestruction.Clear();
}

void AG_Nemesis::LookAtPlayerPos(const uint8& _index)
{
	if (!mTargetToLook || mNecksToRotate.Num() <= _index || mNecksBasicsRotations.Num() <= _index || mNecksBasicsForwards.Num() <= _index) return;
	USceneComponent* _compToRotate = mNecksToRotate[_index];
	if (!_compToRotate) return;
	const FVector _targetPos = mTargetToLook->TargetPosition();
	const FRotator _basicRotator = mNecksBasicsRotations[_index];
	const FVector _neckPos = _compToRotate->GetRelativeLocation() + GetActorLocation();
	const FVector _basicForward = FVector::VectorPlaneProject(mNecksBasicsForwards[_index], FVector::UpVector).GetSafeNormal();
	const FVector _neckToTarget = FVector::VectorPlaneProject(_targetPos - _neckPos, FVector::UpVector).GetSafeNormal();
	const bool _ableToFollowTarget = FVector::DotProduct(_neckToTarget,_basicForward) > FMath::DegreesToRadians(mMaxAngleTargetBasicForward);
	FRotator _newRotator = _ableToFollowTarget ? UKismetMathLibrary::FindLookAtRotation(_neckPos, _targetPos) : _basicRotator;
	_newRotator.Pitch = _basicRotator.Pitch;
	const FRotator _interpRotator = UKismetMathLibrary::RInterpTo_Constant(_compToRotate->GetComponentRotation(), _newRotator, mWorld ? mWorld->DeltaTimeSeconds * CustomTimeDilation : GetWorld()->DeltaTimeSeconds * CustomTimeDilation, mRotationSpeed);
	_compToRotate->SetWorldRotation(_interpRotator);
}

void AG_Nemesis::IncreasePhase()
{
	const uint8& _numberPhases = mAllPhases.Num();
	if (mCurrentPhaseNum + 1 >= _numberPhases) return;
	EndSkills();
	mGodMod = true;
	FTimerManager& _timeManager = mWorld ? mWorld->GetTimerManager() : GetWorld()->GetTimerManager();
	const float _delayTransition = mCurrentPhase.GetTransitionDelay();
	_timeManager.SetTimer(mTransitionPhaseHandle, this, &AG_Nemesis::SetNextPhase, _delayTransition, false);
}

void AG_Nemesis::SetNextPhase()
{
	mCurrentPhaseNum++;
	mCurrentPhase = GetPhase(mCurrentPhaseNum);
	if (!mBrain) return;
	UG_NemesisBrain* _nemesisBrain = StaticCast<UG_NemesisBrain*>(mBrain);
	_nemesisBrain->FillSkillPhase(mCurrentPhase);
	mOnChangePhase.Invoke(mCurrentPhaseNum);
	mGodMod = false;
}

void AG_Nemesis::EndSkills()
{
	UG_NemesisBrain* _nemesisBrain = StaticCast<UG_NemesisBrain*>(mBrain);
	_nemesisBrain->FinishSkills();
}

FG_Phase AG_Nemesis::GetPhase(const uint8& _index)
{
	const uint8& _numberPhases = mAllPhases.Num();
	if (_numberPhases == 0 || _numberPhases <= _index) return FG_Phase();
	else return mAllPhases[_index];
};

void AG_Nemesis::SpawnDestructibleNemesis() 
{
	if (!mDestructibleNemesis)return;
	AG_NemesisDestruct* _nemesisDestruction = (mWorld ? mWorld : GetWorld())->SpawnActor<AG_NemesisDestruct>(mDestructibleNemesis, GetActorLocation(), GetActorRotation());
	if (!_nemesisDestruction)return;
	_nemesisDestruction->OnEndDestruction() += [this]() 
	{
		mOnEndDestruction.Invoke();
	};
}

void AG_Nemesis::TargetTakeDamage(const float& Life, ETypeDamage _typeDamage)
{
	if (mIsDead) return;
	if (mGodMod)
	{
		OnTakeDamage().Invoke(0);
		return;
	}
	float _currentDmg = Life;
	if (mStatsTarget.Armor > 0)
	{
		_currentDmg -= mStatsTarget.Armor;
		mStatsTarget.Armor = mStatsTarget.Armor - Life > 0 ? mStatsTarget.Armor - Life : 0;
	}
	if (_currentDmg > 0)
	{
		mStatsTarget.Life = mStatsTarget.Life - _currentDmg > 0 ? mStatsTarget.Life - _currentDmg : 0;
	}
	OnTakeDamage().Invoke(Life);
	const bool _lastPhase = mCurrentPhaseNum == mAllPhases.Num() - 1;
	if (mStatsTarget.Life <= 0)
	{
		OnDie().Invoke();
		return;
	}
	const bool _watchingHP = mCurrentPhase.NeedHPToEndPhase();
	if (!_watchingHP) return;
	const float _percentHPNeeded = mCurrentPhase.GetPercentHPToEndPhase();
	if ((mStatsTarget.Life / mStatsTarget.MaxLife)*100 <= _percentHPNeeded) IncreasePhase();
}



void AG_Nemesis::CallSpecAchievements(UG_AchievementManager* _achievementManager)
{
	if (!_achievementManager)return;
	Super::CallSpecAchievements(_achievementManager);

	_achievementManager->TryNoHitChallenge();
	_achievementManager->UpdateAchievement(ETypeAchievement::LAST_BOSS);
}