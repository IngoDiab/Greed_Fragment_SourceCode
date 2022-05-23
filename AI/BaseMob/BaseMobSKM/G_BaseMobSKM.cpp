#include "G_BaseMobSKM.h"
#include "Greed/AI/Brain/AnimatedBrain/G_AnimatedBrain.h"
#include "Greed/AI/Component/G_MovementComponent.h"

AG_BaseMobSKM::AG_BaseMobSKM()
{
	mEntityMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Entity Mesh");
	mEntityMesh->SetupAttachment(RootComponent);
}

void AG_BaseMobSKM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RootMotion();
}

void AG_BaseMobSKM::RootMotion()
{
	if (!mEntityMesh)return;
	UAnimInstance* _animInstance = mEntityMesh->GetAnimInstance();
	if (!_animInstance) return;
	FRootMotionMovementParams _paramsRM = _animInstance->ConsumeExtractedRootMotion(1.0f);
	if (!_paramsRM.bHasRootMotion) return;
	FTransform _newTransform = _paramsRM.GetRootMotionTransform();
	AddActorLocalTransform(_newTransform, true);
}