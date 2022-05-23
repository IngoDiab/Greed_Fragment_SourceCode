
#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/BaseMobSKM/G_BaseMobSKM.h"
#include "Greed/AI/DataTable/Tank/DataTank.h"
#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"
#include "G_Tank.generated.h"

UCLASS()
class GREED_API AG_Tank : public AG_BaseMobSKM, public IMovingMob
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Sword", meta = (DisplayName="Sword Mesh"), BlueprintReadWrite)
	UStaticMeshComponent* mSwordMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sword", meta = (DisplayName = "Sword Socket"))
	FName mSwordSocket = "";

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Movement Component"))
	UG_MovementComponent* mMovementSystem = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Steering Component"))
	UG_SteeringComponent* mSteeringComponent = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", VisibleAnywhere, meta = (DisplayName = "Floating Pawn Component"))
	UFloatingPawnMovement* mPawnMovementComponent = nullptr;

public:
	FORCEINLINE virtual UG_MovementComponent* GetMovementComponent() override { return mMovementSystem; };
	FORCEINLINE virtual UG_SteeringComponent* GetSteeringComponent() override { return mSteeringComponent; };
	FORCEINLINE virtual UFloatingPawnMovement* GetFloatingPawnComponent() override { return mPawnMovementComponent; };

	FORCEINLINE virtual void SetMovementComponent(UG_MovementComponent& _moveComponent)  override { mMovementSystem = &_moveComponent; }
	FORCEINLINE virtual void SetSteeringComponent(UG_SteeringComponent& _steeringComponent) override { mSteeringComponent = &_steeringComponent; }
	FORCEINLINE virtual void SetFloatingPawnComponent(UFloatingPawnMovement& _floatingPawnComponent)  override { mPawnMovementComponent = &_floatingPawnComponent; }

	FORCEINLINE float GetMobVelocity() const override{ return mPawnMovementComponent ? mPawnMovementComponent->Velocity.Size() : 0; };
	FORCEINLINE G_ETypeMob GetTypeMob() const override { return mType; };

protected:
	AG_Tank();

	void CreateAIComponents();
	virtual void InitMob() override;
	virtual void InitBrain() override;
	virtual void InitStatsFromDatatable(FG_EnemyData& _data) override;
	virtual void InitComponentsWithData(FG_EnemyData& _data) override;

#pragma region Override
public:
	virtual void SlowSpeed(const float& _speed) override;
	virtual void AddSpeed(const float& _speed) override;
#pragma endregion Override

};
