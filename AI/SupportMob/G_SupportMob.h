#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/BaseMobSM/G_BaseMobSM.h"
#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"
#include "Greed/AI/Interfaces/FleeingMob/FleeingMob.h"
#include "G_SupportMob.generated.h"

class UG_FleeComponent;

UCLASS()
class GREED_API AG_SupportMob : public AG_BaseMobSM, public IFleeingMob
{
	GENERATED_BODY()
	
	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Movement Component"))
	UG_MovementComponent* mMovementSystem = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Steering Component"))
	UG_SteeringComponent* mSteeringComponent = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Flee Component"))
	UG_FleeComponent* mFleeComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Floating Pawn Component")
	UFloatingPawnMovement* mPawnMovementComponent = nullptr;

public:
	FORCEINLINE virtual UG_MovementComponent* GetMovementComponent() override { return mMovementSystem; };
	FORCEINLINE virtual UG_SteeringComponent* GetSteeringComponent() override { return mSteeringComponent; };
	FORCEINLINE virtual UFloatingPawnMovement* GetFloatingPawnComponent() override { return mPawnMovementComponent; };
	FORCEINLINE virtual UG_FleeComponent* GetFleeComponent() override { return mFleeComponent; };

	FORCEINLINE virtual void SetMovementComponent(UG_MovementComponent& _moveComponent)  override { mMovementSystem = &_moveComponent; }
	FORCEINLINE virtual void SetSteeringComponent(UG_SteeringComponent& _steeringComponent) override { mSteeringComponent = &_steeringComponent; }
	FORCEINLINE virtual void SetFloatingPawnComponent(UFloatingPawnMovement& _floatingPawnComponent)  override { mPawnMovementComponent = &_floatingPawnComponent; }
	FORCEINLINE virtual void SetFleeComponent(UG_FleeComponent& _fleeComp) override { mFleeComponent = &_fleeComp; }

	FORCEINLINE float GetMobVelocity() const override { return mPawnMovementComponent ? mPawnMovementComponent->Velocity.Size() : 0; };
	FORCEINLINE G_ETypeMob GetTypeMob() const override { return mType; };

protected:
	AG_SupportMob();
	virtual void InitMob() override;

public:
	virtual void CreateAIComponents();
	virtual void InitBrain();
	virtual void InitStatsFromDatatable(FG_EnemyData& _data) override;
	virtual void InitComponentsWithData(FG_EnemyData& _data) override;

#pragma region Override
public:
	virtual void SlowSpeed(const float& _speed) override;
	virtual void AddSpeed(const float& _speed) override;
#pragma endregion Override
};
