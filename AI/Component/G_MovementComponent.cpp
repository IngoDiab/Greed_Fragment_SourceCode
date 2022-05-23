#include "G_MovementComponent.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"


UG_MovementComponent::UG_MovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UG_MovementComponent::BeginPlay()
{
	Super::BeginPlay();
	mWorld = GetWorld();
	mNavigationSystem = UNavigationSystemV1::GetCurrent(mWorld);
	if (!mOwner) return;
	mAiController = UAIBlueprintHelperLibrary::GetAIController(mOwner);
}

void UG_MovementComponent::BeginDestroy()
{
	mOnMove.Clear();
	mOnPositionReached.Clear();
	Super::BeginDestroy();
}

void UG_MovementComponent::MoveTo()
{
	if (!mCanMove || !mAiController) return;
	mAiController->MoveToLocation(mPosToGo, mUseCustomRadius ? mAcceptanceRadius : -1, false,true,false,true, mNavmeshFilter);
}

void UG_MovementComponent::UpdateMovementSystem()
{
	if (IsAtPos(mPosToGo, mAcceptanceRadius)) mOnPositionReached.Invoke();
	else mOnMove.Invoke();
}

void UG_MovementComponent::RefreshNextPathPoint() 
{
	if (!mOwner) return;
	TArray<FVector> _path = UAIBlueprintHelperLibrary::GetCurrentPathPoints(mAiController);
	if (_path.Num() <= 0) return;
	const FVector _pointOnNavmesh = _path.Num() == 1 ? UAIBlueprintHelperLibrary::GetCurrentPathPoints(mAiController)[0] : UAIBlueprintHelperLibrary::GetCurrentPathPoints(mAiController)[1];
	mNextPathPoint = _pointOnNavmesh + FVector::UpVector * mOwner->GetMobHeight();
	if(mDrawDebug) DrawDebugSphere(mWorld, mNextPathPoint, 20, 20, FColor::Red);
}

void UG_MovementComponent::StopMove()
{
	if (!mAiController)return;
	mAiController->StopMovement();
}

bool UG_MovementComponent::IsAtPos(const FVector& _posToGo, const float& _minDist) const
{
	if (!mOwner) return false;
	return FVector::Dist(mOwner->TargetPosition(), _posToGo) < _minDist;
}

void UG_MovementComponent::LookAtPosition() const
{
	if (!mCanRotate || !mOwner || !mWorld) return;
	const FVector _ownerPos = mOwner->GetActorLocation();
	const FVector _target = FVector(mPosToLook.X, mPosToLook.Y, _ownerPos.Z);
	const FVector _posToLook = _ownerPos + (_target-_ownerPos).GetSafeNormal()* mOffsetDistanceLookAt;
	const FRotator _lookAtRotation = UKismetMathLibrary::FindLookAtRotation(_ownerPos, _posToLook);
	const FRotator _newRotation = UKismetMathLibrary::RInterpTo_Constant(mOwner->GetActorRotation(), _lookAtRotation, mWorld->DeltaTimeSeconds * mOwner->CustomTimeDilation, mSpeedRotate);
	mOwner->SetActorRotation(_newRotation);
}

//BOOMER
FVector UG_MovementComponent::GetRandomPointInRadius(const FVector _center,const float _radius) const
{
	if (!mOwner) return FVector::ZeroVector;
	if (!mNavigationSystem) return mOwner->GetActorLocation();
	FNavLocation _newFLocation;
	mNavigationSystem->GetRandomReachablePointInRadius(_center, _radius, _newFLocation);
	return _newFLocation.Location + FVector::UpVector * mOwner->GetMobHeight();
}
//

bool UG_MovementComponent::IsValid() const
{
	return Super::IsValid() && mAiController && mWorld && mNavigationSystem;
}
