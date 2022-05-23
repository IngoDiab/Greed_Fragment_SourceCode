#include "G_PatrolState.h"
#include "Greed/AI/Boomer/G_BoomerBrain.h"
#include "Greed/AI/Component/G_MovementComponent.h"

void UG_PatrolState::GetUsefulRef()
{
	if (!mBrain) return;
	UG_BoomerBrain* _boomerBrain = Cast<UG_BoomerBrain>(mBrain);
	if (!_boomerBrain) return;
	mMovementComponent = _boomerBrain->GetMovementComponent();
	mSpawnLocation = _boomerBrain->GetStartPos();
	mPatrolRange = _boomerBrain->GetPatrolRange();
}

void UG_PatrolState::GetNewPatrolPoint() const
{
	if (!mMovementComponent) return;
	const FVector _posToGoPatrol = mMovementComponent->GetRandomPointInRadius(mSpawnLocation, mPatrolRange);
	mMovementComponent->SetPosToGo(_posToGoPatrol);
}

void UG_PatrolState::UpdateMovement() 
{
	if (!mMovementComponent) return;
	//mMovementComponent->MoveTo();
	mMovementComponent->RefreshNextPathPoint();
	mMovementComponent->SetPosToLook(mMovementComponent->GetNextPathPoint());
	mMovementComponent->UpdateMovementSystem();
}

/// <summary>
/// Needed because MoveToLocation is sometimes glitched when used on tick (especially w/ CustomPathFollowingComponent)
/// </summary>
void UG_PatrolState::DelegateMove()
{
	FTimerDelegate _delegate;
	_delegate.BindLambda([=]
	{
		if (mMovementComponent) mMovementComponent->MoveTo();
	});
	if (!mWorld)return;
	mWorld->GetTimerManager().SetTimer(mHandle, _delegate, .01f, true);
}

void UG_PatrolState::EndDelegateMove()
{
	if (!mWorld)return;
	mWorld->GetTimerManager().ClearTimer(mHandle);
	if (mMovementComponent) mMovementComponent->StopMove();
}

void UG_PatrolState::InitState(UG_Brain* Ug_Brain)
{
	Super::InitState(Ug_Brain);
	GetUsefulRef();
	OnEnter += [this]() 
	{ 
		GetNewPatrolPoint();
		DelegateMove();
	};
	OnUpdate += [this](float _time) { UpdateMovement(); };
	OnExit += [this]() {EndDelegateMove(); };
}

void UG_PatrolState::BeginDestroy()
{
	EndDelegateMove();
	Super::BeginDestroy();
}