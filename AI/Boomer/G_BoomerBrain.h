#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/Brain/AnimatedBrain/G_AnimatedBrain.h"
#include "Greed/AI/DataTable/Boomer/DataBoomer.h"
#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"
#include "G_BoomerBrain.generated.h"

class UG_MC_DashComponent;
class UG_DetectionComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GREED_API UG_BoomerBrain : public UG_AnimatedBrain, public IMovingMob
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Dash Component"))
	UG_MC_DashComponent* mDashComponent = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Detection Component"))
	UG_DetectionComponent* mDetectionComponent = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Movement Component"))
	UG_MovementComponent* mMovementSystem = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Steering Component"))
	UG_SteeringComponent* mSteeringComponent = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", VisibleAnywhere, meta = (DisplayName = "Floating Pawn Component"))
	UFloatingPawnMovement* mPawnMovementComponent = nullptr;

	UPROPERTY(Category = "Brain | FSM Bool", EditAnywhere, BlueprintReadOnly)
	bool mOtherBoomerAttacking = false;
	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mWaitPatrol = false;

	UPROPERTY(Category = "Brain | GD", EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Min Wait Patrol"))
	float mMinWaitPatrol = 3;
	UPROPERTY(Category = "Brain | GD", EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Max Wait Patrol"))
	float mMaxWaitPatrol = 8;
	UPROPERTY(Category = "Brain | GD", EditAnywhere, meta = (DisplayName = "Patrol Range"))
	float mPatrolRange = 200;
	UPROPERTY(VisibleAnywhere)
	FVector mStartPosition = FVector::ZeroVector;

public:
	FORCEINLINE void SetDashComponent(UG_MC_DashComponent& _dashComp) { mDashComponent = &_dashComp; }
	FORCEINLINE UG_MC_DashComponent* GetDashComponent() const { return mDashComponent; }

	FORCEINLINE void SetDetectionComponent(UG_DetectionComponent& _detectComp) { mDetectionComponent = &_detectComp; }
	FORCEINLINE UG_DetectionComponent* GetDetectionComponent() const { return mDetectionComponent; }

	FORCEINLINE virtual UG_MovementComponent* GetMovementComponent() override { return mMovementSystem; };
	FORCEINLINE virtual UG_SteeringComponent* GetSteeringComponent() override { return mSteeringComponent; };
	FORCEINLINE virtual UFloatingPawnMovement* GetFloatingPawnComponent() override { return mPawnMovementComponent; };

	FORCEINLINE virtual void SetMovementComponent(UG_MovementComponent& _moveComponent)  override { mMovementSystem = &_moveComponent; }
	FORCEINLINE virtual void SetSteeringComponent(UG_SteeringComponent& _steeringComponent) override { mSteeringComponent = &_steeringComponent; }
	FORCEINLINE virtual void SetFloatingPawnComponent(UFloatingPawnMovement& _floatingPawnComponent)  override { mPawnMovementComponent = &_floatingPawnComponent; }

	FORCEINLINE float GetMobVelocity() const { return mPawnMovementComponent ? mPawnMovementComponent->Velocity.Size() : 0; };

public:
	FORCEINLINE bool GetOtherBoomerAttacking() const { return mOtherBoomerAttacking; };
	FORCEINLINE void SetOtherBoomerAttacking(const bool _otherBoomerAttacking) { mOtherBoomerAttacking = _otherBoomerAttacking; };
	FORCEINLINE void SetWaitPatrol(const bool _waiting) { mWaitPatrol = _waiting; };
	FORCEINLINE FVector GetStartPos() const { return mStartPosition; };
	FORCEINLINE float GetPatrolRange() const { return mPatrolRange; };

protected:
	virtual void InitData(FDataMob& _data) override;
	virtual void Update(const float _deltaTime) override;
	virtual void InitFSM() override;
	virtual void InitAnimations() override;
	virtual void UpdateBooleans() override;
	virtual void UpdateAnimations() override;
	virtual void CreateListener() override;
	virtual void CreateMovementListener();
	virtual void CreateFightListener() override;
	virtual void Stun() override;
};
