#include "G_DetectionComponent.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"
#include "Greed/AI/BaseMob/G_BaseMob.h"

UG_DetectionComponent::UG_DetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UG_DetectionComponent::BeginPlay()
{
	Super::BeginPlay();
	InitTarget();
	mOnTargetDetected.AddLambda([this]() 
	{
		mTargetDetected = true;
	});

	mOnTargetLoose += [this]()
	{
		mTargetDetected = false;
	};
}

void UG_DetectionComponent::InitTarget()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller) return;
	ITarget* _target = Cast<ITarget>(_controller->GetPawn());
	if (!_target) return;
	mTarget.SetInterface(_target);
	mTarget.SetObject(_controller->GetPawn());
}

void UG_DetectionComponent::BeginDestroy()
{
	mOnTargetDetected.Clear();
	mOnTargetLoose.Clear();
	Super::BeginDestroy();
}

void UG_DetectionComponent::UpdateDetection()
{
	if (!mTarget)return;
	const bool& _inRange = IsInRange(mTarget->TargetPosition(), mDetectionRange);
	if (!mTargetDetected && _inRange) mOnTargetDetected.Broadcast();
	else if (mTargetDetected && !_inRange) mOnTargetLoose.Invoke();
}

bool UG_DetectionComponent::IsInRange(const FVector& _target, const float& _dist) const
{
	if (!IsValid())return false;
	return FVector::Dist(mOwner->GetActorLocation(), _target) <= _dist;
}
