#include "G_SteeringComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Greed/Manager/GameMode/G_GameMode.h"
#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"
#include "Greed/AI/Component/RepulsionComponent/G_RepulsionComponent.h"

UG_SteeringComponent::UG_SteeringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UG_SteeringComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UG_SteeringComponent::BeginDestroy() 
{
	if (mWorld) 
	{
		FTimerManager& _manager = mWorld->GetTimerManager();
	}
	mOnAddMobNearby.Clear();
	mOnRemoveMobNearby.Clear();
	mOnMobNear.Clear();
	mOnNoMobNear.Clear();
	mOnPathUdpatedSteering.Clear();
	Super::BeginDestroy();
}

void UG_SteeringComponent::Init()
{
	mWorld = GetWorld();
	if (!mWorld)return;
	AG_GameMode* _gameMode = mWorld->GetAuthGameMode<AG_GameMode>();
	if (!_gameMode)return;
	mAIManager = _gameMode->MobManager();
	FTimerManager& _manager = mWorld->GetTimerManager();
	_manager.SetTimer(mRefreshSteering, this, &UG_SteeringComponent::RefreshSteering, mRefreshRateSteering, true);

	mOnAddMobNearby += [this](AG_Mob* _mob) 
	{
		Add<AG_Mob>(*_mob, mNearMobs);
		_mob->OnPreDie().AddDynamic(this, &UG_SteeringComponent::RemoveMobFromNearbyOnDie);
	};

	mOnRemoveMobNearby += [this](AG_Mob* _mob) 
	{
		Remove<AG_Mob>(*_mob, mNearMobs);
		_mob->OnPreDie().RemoveDynamic(this, &UG_SteeringComponent::RemoveMobFromNearbyOnDie);
	};
}

void UG_SteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (mUseDebugs) DrawDebugs();
}

void UG_SteeringComponent::DrawDebugs()
{
	if (!mOwner) return;
}

void UG_SteeringComponent::GetNearMobs() 
{
	if (!mAIManager || !mOwner) return;
	const TArray<AG_Mob*> _allMobs = mAIManager->GetAllMob();
	if (_allMobs.Num() == 0) return;
	for (AG_Mob* _mob : _allMobs)
	{
		if (_mob == mOwner || !_mob || _mob->IsActorBeingDestroyed()) continue;
		const FVector _mobPos = FVector::VectorPlaneProject(_mob->GetActorLocation(), mOwner->GetActorUpVector());
		const FVector _ownerPos = mOwner->GetActorLocation();
		const float _distance = FVector::Dist(_mobPos, _ownerPos);

		if (_distance <= mOwner->GetRadius() + _mob->GetRadius() && !IsMobInSteeringOnes(*_mob)) 
		{
			mOnAddMobNearby.Invoke(_mob);
		}

		else if (_distance >= mOwner->GetEndRadius() + _mob->GetEndRadius() && IsMobInSteeringOnes(*_mob))
		{
			mOnRemoveMobNearby.Invoke(_mob);
		}
	}
}

void UG_SteeringComponent::RemoveMobFromNearbyOnDie(AG_Mob* _mob)
{
	Remove<AG_Mob>(*_mob, mNearMobs);
	IMovingMob* _movingMob = Cast<IMovingMob>(_mob);
	if (!_movingMob) return;
	UG_SteeringComponent* _mobSteeringComp = _movingMob->GetSteeringComponent();
	if (!_mobSteeringComp) return;
	_mob->OnPreDie().RemoveDynamic(this, &UG_SteeringComponent::RemoveMobFromNearbyOnDie);
}

void UG_SteeringComponent::RefreshSteering()
{
	GetNearMobs();
	ResetSteering();
	FVector _currentAvoidingForce = FVector::ZeroVector;
	CalculateMobsRepulsions(_currentAvoidingForce);
	CalculateActorsComponentRepulsions(_currentAvoidingForce);
	RefreshForce(_currentAvoidingForce);
}

void UG_SteeringComponent::ResetSteering()
{
	if (mNearMobs.Num() != 0 || mRepulsiveActorsComponent.Num() != 0) return;
	mBufferedAvoidementForce = FVector::ZeroVector;
	mAvoidementForce = FVector::ZeroVector;
	return;
}

void UG_SteeringComponent::CalculateMobsRepulsions(FVector& _resultForce) 
{
	const int _nearMobsCount = mNearMobs.Num();
	if (_nearMobsCount > 0) 
	{
		mOnMobNear.Invoke();
	}
	else 
	{
		mOnNoMobNear.Invoke();
		return;
	}
	for (AG_Mob* _mob : mNearMobs)
	{
		if (!_mob)continue;
		const FVector _mobPos = _mob->GetActorLocation();
		const FVector _ownerPos = mOwner->GetActorLocation();
		_resultForce += ClampSeparationForce(_ownerPos - _mobPos);
	}
}

void UG_SteeringComponent::CalculateActorsComponentRepulsions(FVector& _resultForce)
{
	for (UG_RepulsionComponent* _actorComponent : mRepulsiveActorsComponent)
	{
		if (!_actorComponent)continue;
		const AActor* _owner = _actorComponent->GetOwner();
		if (!_owner) continue;
		const FVector _ownerPos = mOwner->GetActorLocation();
		const FVector _actorPos = FVector(_owner->GetActorLocation().X, _owner->GetActorLocation().Y, _ownerPos.Z);
		_resultForce += ClampSeparationForce(_ownerPos - _actorPos);
	}
}

void UG_SteeringComponent::RefreshForce(const FVector& _currentAvoidingForce)
{
	if (!mAvoidementForce.IsZero() && FVector::DotProduct(mAvoidementForce, _currentAvoidingForce) < 0) return;
	mBufferedAvoidementForce = mAvoidementForce;
	mAvoidementForce = _currentAvoidingForce;
	mAvoidementForce.Normalize();
}

FVector UG_SteeringComponent::ClampSeparationForce(const FVector& _baseVector) const
{
	if (!mOwner) return FVector::ZeroVector;
	return FVector::VectorPlaneProject(_baseVector.GetSafeNormal(), mOwner->GetActorUpVector());
}

bool UG_SteeringComponent::IsMobInSteeringOnes(const AG_Mob& _mob) const
{
	if (mNearMobs.Num() == 0) return false;
	return Exist<AG_Mob>(_mob, mNearMobs);
}
