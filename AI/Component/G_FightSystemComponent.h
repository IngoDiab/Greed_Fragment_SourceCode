#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/G_BaseAiComponent.h"
#include "Greed/Utils/Action/Action.h"
#include "G_FightSystemComponent.generated.h"

class UG_Skill;
class UG_SkillComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GREED_API UG_FightSystemComponent : public UG_BaseAiComponent
{
	GENERATED_BODY()

	Action<> mOnTargetInAttackRange = Action<>();
	Action<> mOnTargetNotInAttackRange = Action<>();
	Action<uint8> mOnAttack = Action<uint8>();
	Action<uint8> mOnAttackEnded = Action<uint8>();
	Action<> mOnEndCD = Action<>();
	Action<> mOnBeginCDAnim = Action<>();
	Action<> mOnEndCDAnim = Action<>();

 protected:
	UPROPERTY(EditAnywhere)
	TArray<int> mAutoRepeatableSkillsSlots;
	
	UPROPERTY()
	UG_SkillComponent* mSkillComponent = nullptr;

	UPROPERTY()
	UG_Skill* mLastSkillUsed = nullptr;

	UPROPERTY(VisibleAnywhere)
	TScriptInterface<ITarget> mTarget = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "Show Debugs"))
	bool mUseDebug = true;
	
	UPROPERTY(VisibleAnywhere, Category = "Debug", meta = (DisplayName = "Player In Range"))
	bool mPlayerInRange = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Debug", meta = (DisplayName = "Is Attacking"))
	bool mIsAttacking = false;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Need To See Player"))
	bool mNeedToSeePlayer = true;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Need Player Ahead"))
	bool mNeedPlayerAhead = true;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Player ahead setting"))
	float mTargetAheadAdjustement = .9f;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Min Attack Range"))
	float mMinAttackRange = 100;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Max Attack Range"))
	float mMaxAttackRange = 150;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Use Sphere Cast"))
	bool mUseSphereCast = false;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Radius Sphere Cast", EditCondition = "mUseSphereCast", EditConditionHides))
	float mRadiusSphereCast = 25;

public:
	FORCEINLINE Action<>& OnTargetInAttackRange() { return mOnTargetInAttackRange; };
	FORCEINLINE Action<>& OnTargetNotInAttackRange() { return mOnTargetNotInAttackRange; };
	FORCEINLINE Action<uint8>& OnAttack() { return mOnAttack; };
	FORCEINLINE Action<uint8>& OnAttackEnded() { return mOnAttackEnded; };
	FORCEINLINE Action<>& OnEndCD() { return mOnEndCD; };
	FORCEINLINE Action<>& OnBeginCDAnim() { return mOnBeginCDAnim; };
	FORCEINLINE Action<>& OnEndCDAnim() { return mOnEndCDAnim; };

	FORCEINLINE TScriptInterface<ITarget> GetTarget() const { return mTarget; };
	FORCEINLINE FVector GetTargetPos() const { return mTarget ? mTarget->TargetPosition() : FVector::ZeroVector; };

	FORCEINLINE UG_Skill* GetLastSkillUsed() const { return mLastSkillUsed; };
	FORCEINLINE void SetMinAttackRange(const float& _range) { mMinAttackRange = _range; };
	FORCEINLINE void SetMaxAttackRange(const float& _range) { mMaxAttackRange = _range; };
	FORCEINLINE float GetMaxAttackRange() const { return mMaxAttackRange; };

	FORCEINLINE virtual bool IsValid() const override { return Super::IsValid() && mTarget; };

public:
	UG_FightSystemComponent();

private:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void InitOwner() override;
	void InitTarget();
	bool CheckAttackRange(const float& _attackRange) const;
	bool IsVisionObstructed() const;

public:
	void InitSkill();
	void UpdateFightSystem();
	void UseAutoRepeatableSkills();
	bool IsPlayerAhead() const;
	void Attack(const uint8& _slotSkill);
	void EndAttack(const uint8& _slotSkill);
	void UseSkill(const uint8& _slotSkill);
	bool MobInCD() const;
	void DrawDebugs() const;
};

