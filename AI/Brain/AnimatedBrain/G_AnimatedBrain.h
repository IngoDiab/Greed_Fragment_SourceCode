#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/Brain/G_Brain.h"
#include "G_AnimatedBrain.generated.h"

class UG_AnimBaseMob;

UCLASS()
class GREED_API UG_AnimatedBrain : public UG_Brain
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Animations"))
	UG_AnimBaseMob* mAnimations = nullptr;

	//MAYBE IN BRAIN
	UPROPERTY(Category = "Brain | FSM Bool", EditAnywhere, BlueprintReadOnly)
	bool mDetectPlayer = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mAtPlayerRangeButNotTooClose = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mPlayerTooFar = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mAtRangeAttack = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mAtRangeAttackAndAim = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mWaitCooldown = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mIsInCD = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mCDWithoutAnim = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mIsInCDAnim = false;
	//
	
protected:
	virtual void Init() override;
	virtual void InitOwner() override;
	virtual void InitAnimations();

protected:
	virtual void Update(const float _deltaTime) override;
	//MAYBE IN BRAIN
	virtual void UpdateBooleans();
	//
	virtual void UpdateAnimations();

	virtual void CreateFightListener() override;
};
