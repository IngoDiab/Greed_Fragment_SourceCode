#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "G_MovementNotify.generated.h"

class UG_AnimBaseMob;

UCLASS()
class GREED_API UG_MovementNotify : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Can Move"))
		bool mCanMove = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Can Rotate"))
		bool mCanRotate = false;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};