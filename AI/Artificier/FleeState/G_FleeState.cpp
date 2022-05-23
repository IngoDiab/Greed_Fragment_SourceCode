#include "G_FleeState.h"
#include "Greed/AI/Component/G_MovementComponent.h"
#include "Greed/AI/Component/G_FleeComponent.h"
#include "Greed/AI/Interfaces/FleeingMob/FleeingMob.h"
#include "Greed/AI/Brain/G_Brain.h"
#include "Greed/Utils/Debug/Debug.h"

void UG_FleeState::GetUsefulRef()
{
	if (!mBrain) return;
	IFleeingMob* _fleeingMobBrain = Cast<IFleeingMob>(mBrain);
	if (!_fleeingMobBrain) return;
	mMovementComponent = _fleeingMobBrain->GetMovementComponent();
	mFleeComponent = _fleeingMobBrain->GetFleeComponent();
}

void UG_FleeState::Flee()
{
	if (!mFleeComponent || !mMovementComponent) return;
	const FVector _posToFlee = mFleeComponent->FindFleeingPoint();
	mMovementComponent->SetPosToGo(_posToFlee);
	//mMovementComponent->MoveTo();
	mMovementComponent->RefreshNextPathPoint();
	mMovementComponent->SetPosToLook(mMovementComponent->GetNextPathPoint());
	mMovementComponent->UpdateMovementSystem();
}

/// <summary>
/// Needed because MoveToLocation is sometimes glitched when used on tick (especially w/ CustomPathFollowingComponent)
/// </summary>
void UG_FleeState::DelegateMove() 
{
	FTimerDelegate _delegate;
	_delegate.BindLambda([=]
	{
		if(mMovementComponent) mMovementComponent->MoveTo();
	});
	if (!mWorld)return;
	mWorld->GetTimerManager().SetTimer(mHandle, _delegate, .01f, true);
}

void UG_FleeState::EndDelegateMove()
{
	if (!mWorld)return;
	mWorld->GetTimerManager().ClearTimer(mHandle);
	if (mMovementComponent) mMovementComponent->StopMove();
}

void UG_FleeState::InitState(UG_Brain* Ug_Brain)
{
	Super::InitState(Ug_Brain);
	GetUsefulRef();
	OnEnter += [this]() {DelegateMove(); };
	OnUpdate += [this](float _time){Flee();};
	OnExit += [this]() {EndDelegateMove();};
}

void UG_FleeState::BeginDestroy()
{
	EndDelegateMove();
	Super::BeginDestroy();
}