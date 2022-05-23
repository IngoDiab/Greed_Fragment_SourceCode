#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/G_Mob.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "G_BaseMob.generated.h"

class UG_FightSystemComponent;
class UG_MovementComponent;
class UG_FSMComponent;
class UG_Brain;
class UG_SkillComponent;

UCLASS()
class GREED_API AG_BaseMob : public AG_Mob
{
	GENERATED_BODY()

protected:
	DECLARE_EVENT_OneParam(AG_BaseMob, ChangeTarget, ITarget*)
	ChangeTarget mOnChangeTarget;

protected:
	UPROPERTY(Category = "Brain", EditAnywhere, meta = (DisplayName = "Brain", AllowPrivateAccess = true), BlueprintReadOnly)
		UG_Brain* mBrain = nullptr;

	UPROPERTY(Category = "Brain | FSM", VisibleAnywhere)
		UG_FSMComponent* mFSM = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Fight Component"))
		UG_FightSystemComponent* mFightSystem = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Skill Component"))
		UG_SkillComponent* mSkillComponent = nullptr;

public:
	FORCEINLINE ChangeTarget& OnChangeTarget() { return mOnChangeTarget; };

public:
	FORCEINLINE UG_Brain* GetBrain() const { return mBrain; }

public:
	AG_BaseMob();
	void PostInitProperties() override;

protected:
	virtual void BeginDestroy() override;

protected:
	virtual void ClearAction() override;
	virtual void CreateAIComponents();
	virtual void InitBrain();
	virtual void InitComponentsWithData(FG_EnemyData& _data) override;

public:
	void ChangeTarget(ITarget* _player);
	virtual void FreezeMob(const bool& _freeze) override;
};