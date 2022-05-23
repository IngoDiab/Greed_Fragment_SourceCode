#include "G_MovementNotify.h"
#include "Greed/AI/Animations/G_AnimBaseMob.h"

void UG_MovementNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (!MeshComp)return;
	UG_AnimBaseMob* _anims = Cast<UG_AnimBaseMob>(MeshComp->GetAnimInstance());
	if (!_anims) return;
	_anims->LockMovements(mCanMove, mCanRotate);
}

void UG_MovementNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (!MeshComp)return;
	UG_AnimBaseMob* _anims = Cast<UG_AnimBaseMob>(MeshComp->GetAnimInstance());
	if (!_anims) return;
	_anims->LockMovements(true, true);
}