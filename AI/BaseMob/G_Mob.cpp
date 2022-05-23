#include "G_Mob.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "Greed/UI/Components/ComponentEnemy/G_UI_ComponentEnemy.h"
#include "Greed/Drop/G_DropComponent.h"
#include "Greed/Effects/Component/G_EffectComponent.h"
#include "Greed/Manager/GameMode/G_GameMode.h"
#include "Greed/AI/Manager/G_AIManager.h"
#include "Greed/Manager/FXManager/G_FXManager.h"
#include "NiagaraComponent.h"

#include "Greed/UI/DamageActor/G_DamageActor.h"

#include "Greed/Manager/Achievements/G_AchievementManager.h"
#include "Greed/Manager/GameInstance/G_GameInstance.h"
#include "Greed/UI/Audio/G_MainMenuAudio.h"
#include "Greed/UI/MainMenu/MainMenuPlay/G_MainMenuPlay.h"

#pragma region Stats

bool AG_Mob::IsValid() const
{
	return mWorld != nullptr;
}

float AG_Mob::Life() const
{
	return mStatsTarget.Life;
}

float AG_Mob::MaxLife() const
{
	return mStatsTarget.MaxLife;
}

float AG_Mob::Speed() const
{
	return mStatsTarget.Speed;
}

float AG_Mob::MaxSpeed() const
{
	return mStatsTarget.MaxSpeed;
}

float AG_Mob::DamageResistance() const
{
	return mStatsTarget.DamageResistance;
}

float AG_Mob::MaxDamageResistance() const
{
	return mStatsTarget.MaxDamageResistance;
}

float AG_Mob::Armor() const
{
	return mStatsTarget.Armor;
}

float AG_Mob::MaxArmor() const
{
	return mStatsTarget.MaxArmor;
}

void AG_Mob::AddArmor(const float& Armor)
{
	mStatsTarget.Armor = mStatsTarget.Armor + Armor > mStatsTarget.MaxArmor ? mStatsTarget.MaxArmor : mStatsTarget.Armor + Armor;
	mOnReceiveArmor.Invoke(mStatsTarget.Armor);
}

void AG_Mob::TargetTakeDamage(const float& Life, ETypeDamage _typeDamage)
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
	if (_typeDamage == ETypeDamage::PureDamage || _typeDamage == ETypeDamage::Bleed) mOnMobHitPureDamage.Invoke(Life);
	OnTakeDamage().Invoke(Life);
	if (mStatsTarget.Life <= 0)
	{
		FString _name = GetName() + "\n";
		FString _nameFile = "DebugMob.txt";
		FString _path = FPaths::Combine(FPaths::ProjectDir(), _nameFile);
		FFileHelper::SaveStringToFile(_name, *_path, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
		mOnPreDie.Broadcast(this);
		OnDie().Invoke();
		return;
	}

}

void AG_Mob::TargetTakeDamage(const float& Life, ETypeDamage _typeDamage, const bool& _forceTakeDamageEvenGodMod)
{
	if ((mGodMod && !_forceTakeDamageEvenGodMod) || mIsDead) return;
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
	if (_typeDamage == ETypeDamage::PureDamage) mOnMobHitPureDamage.Invoke(Life);
	OnTakeDamage().Invoke(Life);
	if (mStatsTarget.Life <= 0)
	{
		mOnPreDie.Broadcast(this);
		OnDie().Invoke();
		return;
	}

}

void AG_Mob::AddLife(const float& Life)
{
	mStatsTarget.Life = mStatsTarget.Life + Life < mStatsTarget.MaxLife
		? mStatsTarget.Life + Life
		: mStatsTarget.MaxLife;
	mOnHealing.Invoke(Life);
}

void AG_Mob::SetMaxLife(const float& MaxLife)
{
	mStatsTarget.MaxLife = MaxLife;
}

void AG_Mob::SlowSpeed(const float& Speed)
{
	
}

void AG_Mob::AddSpeed(const float& Speed)
{

}

void AG_Mob::SetMaxSpeed(const float& MaxSpeed)
{
	mStatsTarget.MaxSpeed = MaxSpeed;
}

UG_EffectComponent* AG_Mob::GetEffectComponent() const
{
	return mEffectComponent;
}
#pragma endregion Stats

#pragma region Debug
bool AG_Mob::ShowDebug() const
{
	return mVisualDebug.IsShowDebug();
}

float AG_Mob::DebugSize() const
{
	return mVisualDebug.DebugSize();
}

float AG_Mob::DebugHeight() const
{
	return mVisualDebug.DebugHeight();
}

FColor AG_Mob::DebugValidColor() const
{
	return mVisualDebug.DebugValidColor();
}

FColor AG_Mob::DebugErrorColor() const
{
	return mVisualDebug.DebugErrorColor();
}
#pragma endregion Debug

#pragma region ITarget
FVector AG_Mob::TargetPosition() const
{
	return GetActorLocation();
}

void AG_Mob::OnDrawGizmos()
{
	if (!ShowDebug())return;
	const FVector _posMobDebug = GetActorLocation();
	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), _posMobDebug, mRadius, 360, FColor::Orange, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), _posMobDebug, mEndRadius, 360, FColor::Green, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
	const FColor& _color = IsValid() ? DebugValidColor() : DebugErrorColor();
	const FVector& _location = GetActorLocation();
	const FVector& _offset = _location + FVector::UpVector * DebugHeight();
	DrawDebugSphere(mWorld, _offset, DebugSize(), 16, _color);
	DrawDebugLine(mWorld, _location, _offset, _color);
	mOnDrawGizmosEvent.Invoke();
}
#pragma endregion ITarget

AG_Mob::AG_Mob()
{
#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	mCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule Entity");
	RootComponent = mCapsuleComponent;

	mUiComponent = CreateDefaultSubobject<UG_UI_ComponentEnemy>(TEXT("UI Component"));
	mUiComponent->SetupAttachment(RootComponent);

	mDropComponent = CreateDefaultSubobject<UG_DropComponent>(TEXT("Drop Component"));
	AddOwnedComponent(mDropComponent);

	mNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	mNiagaraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	mEffectComponent = CreateDefaultSubobject<UG_EffectComponent>(TEXT("Effect Component"));
	AddOwnedComponent(mEffectComponent);
}

void AG_Mob::BeginPlay()
{
	Super::BeginPlay();
	InitMob();
	OnSpawnSFX();
	OnSpawnVFX();
	SpawnAnimation();
}

void AG_Mob::ClearAction()
{
	mOnTakeDamage.Clear();
	mOnHealing.Clear();
	mOnPreDie.Clear();
	mOnDie.Clear();
	mOnReceiveArmor.Clear();
	mOnDrawGizmosEvent.Clear();
	mOnStun.Clear();
	mOnEndStun.Clear();
}

void AG_Mob::CreateDamageActor(float _damage, bool _godMode)
{
	AG_DamageActor* _dActor = GetWorld()->SpawnActor<AG_DamageActor>(mDamageActorClass, this->GetTransform());
	if (!_dActor) return;
	_dActor->Init(NumberType::Damage, _damage, _godMode);
}

void AG_Mob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if WITH_EDITOR
	OnDrawGizmos();
#endif
}

void AG_Mob::SpawnAnimation() 
{
	SpawnFX(mNameFXSpawn);
}

void AG_Mob::InitMob()
{
	mWorld = GetWorld();
	mAiController = UAIBlueprintHelperLibrary::GetAIController(this);
	InitEvents();
	InitRefs();
	AddToMobManager(true);
}

void AG_Mob::InitStatsFromDatatable(FG_EnemyData& _data)
{
	mStatsTarget.MaxLife = mStatsTarget.Life = _data.GetMaxLife();
	mStatsTarget.Armor = _data.GetArmorAtSpawn();
	mStatsTarget.MaxArmor = _data.GetMaxArmor();
}

void AG_Mob::InitComponentsWithData(FG_EnemyData& _data)
{
	if (!mUiComponent) return;
	mUiComponent->Init(this);
}

void AG_Mob::InitEvents()
{
	OnTakeDamage() += [this](float _damage) {CreateDamageActor(_damage, IsInGodMod()); };
	OnDie() += [this]()
	{
		OnDeathSFX();
		OnDeathVFX();
		AddToMobManager(false);
		BeginDestroyAfterDeath();
		CallAchievements();
	};
}

void AG_Mob::InitRefs() 
{
	if (!mWorld)return;
	AG_GameMode* _gm = mWorld->GetAuthGameMode<AG_GameMode>();
	if (!_gm)return;
	mMobManager = _gm->MobManager();
	mFXManager = _gm->FXManager();
}

void AG_Mob::AddToMobManager(const bool& _add)
{
	if (!mMobManager)
	{
		if (!mWorld)return;
		AG_GameMode* _gm = mWorld->GetAuthGameMode<AG_GameMode>();
		if (!_gm)return;
		mMobManager = _gm->MobManager();
	}
	if (mMobManager) _add ? mMobManager->AddMob(*this) : mMobManager->RemoveMob(*this);
}

void AG_Mob::BeginDestroyAfterDeath() 
{
	mIsDead = true;
	SpawnFX(mNameFXDeath);
	DropSouls();
	FreezeMob(true);
	if (!mDestroyAfterDeath || !mWorld) return;
	if (mDelayDestroy <= 0) EndDestroyAfterDeath();
	else mWorld->GetTimerManager().SetTimer(mDelayDestroyHandler, this, &AG_Mob::EndDestroyAfterDeath, mDelayDestroy, false);
}

void AG_Mob::EndDestroyAfterDeath() 
{
	ClearTimeManager(mDelayDestroyHandler);
	Destroy();
}

void AG_Mob::FreezeMob(const bool& _freeze) 
{
	CustomTimeDilation = !_freeze;
}

void AG_Mob::DropSouls()
{
	if (!mDropComponent) return;
	const FVector _posToDrop = GetActorLocation() - FVector::UpVector * GetMobHeight();
	mDropComponent->DropSouls(_posToDrop, UKismetMathLibrary::RandomUnitVector(), false);
}

void AG_Mob::SpawnFX(const FString& _keyFX) const
{
	if (!mFXManager)return;
	const FVector _positionFX = GetActorLocation() - GetMobHeight() * FVector::UpVector;
	const FRotator _rotationFX = GetActorRotation();
	mFXManager->SpawnFXMob(mType, _keyFX, _positionFX, _rotationFX);
}

void AG_Mob::SetGodMod(float _time)
{
	mGodMod = true;
	FTimerHandle _handle;
	FTimerDelegate _delegate;
	_delegate.BindLambda([=]
	{
		mGodMod = false;
	});
	(mWorld ? mWorld : GetWorld())->GetTimerManager().SetTimer(_handle, _delegate, _time, false);
}

void AG_Mob::Stun(float _stunDuration)
{
	FreezeMob(true);
	mIsStun = true;
	mOnStun.Invoke();
	if (_stunDuration == -1) 
	{
		ClearTimeManager(mStunTimer);
		return;
	}
	(mWorld? mWorld : GetWorld())->GetTimerManager().SetTimer(mStunTimer, this, &AG_Mob::EndStun, _stunDuration, false);
}

void AG_Mob::EndStun()
{
	FreezeMob(false);
	mIsStun = false;
	mOnEndStun.Invoke();
}

void AG_Mob::ClearTimeManager(FTimerHandle& _handler)
{
	FTimerManager& _timeManager = GetWorld()->GetTimerManager();
	if(_timeManager.TimerExists(_handler)) _timeManager.ClearTimer(_handler);
}


void AG_Mob::CallAchievements()
{
	UG_GameInstance* _gInstance = Cast<UG_GameInstance>(GetWorld()->GetGameInstance());
	if (!_gInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("MOB : No GI Found"));

		return;
	}
	UG_AchievementManager* _achievementManager = _gInstance->GetAchievementManager();
	if (!_achievementManager)
	{
		UE_LOG(LogTemp, Error, TEXT("MOB : No AchievementManager Found"));

		return;
	}

	CallSpecAchievements(_achievementManager);
}

void AG_Mob::CallSpecAchievements(UG_AchievementManager* _achievementManager)
{
	if (!_achievementManager) return;
	_achievementManager->UpdateAchievement(ETypeAchievement::KILL_100);
	_achievementManager->UpdateAchievement(ETypeAchievement::KILL_1000);
}