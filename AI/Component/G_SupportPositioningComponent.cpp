#include "G_SupportPositioningComponent.h"

void UG_SupportPositioningComponent::BeginPlay()
{
	mOwner->OnActorBeginOverlap.AddDynamic(this, &UG_SupportPositioningComponent::InRange);
	mOwner->OnActorEndOverlap.AddDynamic(this, &UG_SupportPositioningComponent::OutRange);
}

void UG_SupportPositioningComponent::InRange(AActor* _thisActor, AActor* _otherActor)
{
	AG_BaseMob* _tmp = Cast<AG_BaseMob>(_otherActor);
	if(!_tmp)return;
	mAllMob.Add(_tmp);
	mCurrentHealerWeight += _tmp->GetHealerWeight();
}

void UG_SupportPositioningComponent::OutRange(AActor* _thisActor, AActor* _otherActor)
{
	AG_BaseMob* _tmp = Cast<AG_BaseMob>(_otherActor);
	if(!_tmp)return;
	int _healerWeightLoss = _tmp->GetHealerWeight();
	mCurrentHealerWeight -= _healerWeightLoss;
	if(_healerWeightLoss > mCurrentHealerWeight)
		FollowMob.Invoke(_tmp->GetActorLocation());
}
