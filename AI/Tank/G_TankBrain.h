#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/Brain/AnimatedBrain/G_AnimatedBrain.h"
#include "Greed/AI/DataTable/Tank/DataTank.h"
#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"
#include "G_TankBrain.generated.h"

UCLASS()
class GREED_API UG_TankBrain : public UG_AnimatedBrain, public IMovingMob
{
	GENERATED_BODY()

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Movement Component"))
	UG_MovementComponent* mMovementSystem = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Steering Component"))
	UG_SteeringComponent* mSteeringComponent = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", VisibleAnywhere, meta = (DisplayName = "Floating Pawn Component"))
	UFloatingPawnMovement* mPawnMovementComponent = nullptr;

	UPROPERTY(Category = "Brain", VisibleAnywhere, meta = (DisplayName = "Can Relocate Steering"))
	bool mCanRelocateSteering = true;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere)
	bool mRelocateSteering = false;

public:
	FORCEINLINE virtual UG_MovementComponent* GetMovementComponent() override { return mMovementSystem; };
	FORCEINLINE virtual UG_SteeringComponent* GetSteeringComponent() override { return mSteeringComponent; };
	FORCEINLINE virtual UFloatingPawnMovement* GetFloatingPawnComponent() override { return mPawnMovementComponent; };

	FORCEINLINE virtual void SetMovementComponent(UG_MovementComponent& _moveComponent)  override { mMovementSystem = &_moveComponent; }
	FORCEINLINE virtual void SetSteeringComponent(UG_SteeringComponent& _steeringComponent) override { mSteeringComponent = &_steeringComponent; }
	FORCEINLINE virtual void SetFloatingPawnComponent(UFloatingPawnMovement& _floatingPawnComponent)  override { mPawnMovementComponent = &_floatingPawnComponent; }

protected:
	virtual void InitFSM() override;
	virtual void InitAnimations() override;
	virtual void CreateListener() override;
	virtual void CreateMovementListener();
	virtual void CreateSteeringListener();
	virtual void CreateFightListener() override;
	virtual void Update(const float _deltaTime) override;
	virtual void UpdateAnimations() override;

public:
	virtual bool IsValid() const override;

	virtual void InitData(FDataMob& _data);
	virtual void Stun() override;
};
