#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "G_AttackNotify.generated.h"

class UG_AnimBaseMob;

UCLASS()
class GREED_API UG_AttackNotify : public UAnimNotifyState
{
	GENERATED_BODY()

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Slot Skill to use"))
	uint8 mSlotSkill = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "State Machine Name"))
	FName mStateMachineName = FName();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Attack State Name"))
	FName mAttackStateName = FName();

	UPROPERTY()
	bool mNeedToRetryBegin = false;
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

private:
	bool Begin(USkeletalMeshComponent& _meshComp);
};
