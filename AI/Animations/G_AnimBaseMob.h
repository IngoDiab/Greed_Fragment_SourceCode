#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "G_AnimBaseMob.generated.h"

class UG_FightSystemComponent;
class UG_MovementComponent;

UCLASS()
class GREED_API UG_AnimBaseMob : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UG_MovementComponent* mMoveComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UG_FightSystemComponent* mFightComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	float mVelocity = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool mCooldown = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool mIsStun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	TArray<bool> mAttacksAnimations = TArray<bool>();

public:
	FORCEINLINE void SetMovementSystem(UG_MovementComponent& _movementComp) { mMoveComp = &_movementComp; };
	FORCEINLINE void SetFightSystem(UG_FightSystemComponent& _fightComp) { mFightComp = &_fightComp; };
	FORCEINLINE void SetVelocity(const float& _velocity) { mVelocity = _velocity; };	

	FORCEINLINE void SetIsStun(const bool& _isStun) { mIsStun = _isStun; };
	FORCEINLINE void SetAnimCoolDown(const bool& _cooldown) { mCooldown = _cooldown; };
	FORCEINLINE void ActivateAttackAnimation(const uint8& _slotAttackAnim, const bool& _activate) { mAttacksAnimations[_slotAttackAnim] = _activate; };

public:
	void InitAttackAnimationsSlots(const uint8& _nbSlots);
	void TriggerAnimAttack(const uint8& _slotSkill);
	void LockMovements(const bool& _canMove, const bool& _canRotate);
	void BeginAnimCooldown();
	void EndAnimCooldown();
	float GetStateWeight(FName _stateMachineName, FName _stateName);
};
