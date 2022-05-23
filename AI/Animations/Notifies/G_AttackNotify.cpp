#include "G_AttackNotify.h"
#include "Greed/AI/Animations/G_AnimBaseMob.h"

void UG_AttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	mNeedToRetryBegin = !Begin(*MeshComp);
}

void UG_AttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	if (mNeedToRetryBegin) mNeedToRetryBegin = !Begin(*MeshComp);
}

/// <summary>
/// Excute notify's begin
/// </summary>
/// <param name="_meshComp"> : Mesh Component</param>
/// <returns>true if the attack need to be retry, false if it's done</returns>
bool UG_AttackNotify::Begin(USkeletalMeshComponent& _meshComp)
{
	UG_AnimBaseMob* _anims = Cast<UG_AnimBaseMob>(_meshComp.GetAnimInstance());
	if (!_anims) return false;
	float _weight = _anims->GetStateWeight(mStateMachineName, mAttackStateName);
	if (_weight < 1) return false;
	_anims->TriggerAnimAttack(mSlotSkill);
	return true;
}
