#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/BaseMobSKM/G_BaseMobSKM.h"
#include "Greed/AI/DataTable/Boomer/DataBoomer.h"
#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"
#include "G_Boomer.generated.h"

class UG_BoomerHandler;
class AG_BomberSpawner;
class UG_DetectionComponent;
class UG_MC_DashComponent;

UCLASS()
class GREED_API AG_Boomer : public AG_BaseMobSKM, public IMovingMob
{
	GENERATED_BODY()

	UPROPERTY(Category = "Boomer Handler", VisibleAnywhere, meta = (DisplayName = "Boomer Handler"))
	UG_BoomerHandler* mBoomerHandler = nullptr;

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

	UPROPERTY()
	AG_BomberSpawner* mSpawner = nullptr;

	UPROPERTY(Category="Boomer",VisibleAnywhere, meta = (DisplayName = "Spawned", AllowPrivateAccess = true),BlueprintReadOnly)
	bool mIsSpawned = false;
	
	UPROPERTY(Category = "Boomer", meta = (DisplayName = "Growth Speed", AllowPrivateAccess = true), BlueprintReadOnly)
	FVector mMaxGrowth = FVector::OneVector;

	UPROPERTY(Category="Boomer",EditAnywhere, meta = (DisplayName = "Growth Speed", AllowPrivateAccess = true),BlueprintReadOnly)
	float mGrowthSpeed = 10;

public:
	FORCEINLINE virtual UG_MovementComponent* GetMovementComponent() override { return mMovementSystem; };
	FORCEINLINE virtual UG_SteeringComponent* GetSteeringComponent() override { return mSteeringComponent; };
	FORCEINLINE virtual UFloatingPawnMovement* GetFloatingPawnComponent() override { return mPawnMovementComponent; };

	FORCEINLINE virtual void SetMovementComponent(UG_MovementComponent& _moveComponent)  override { mMovementSystem = &_moveComponent; }
	FORCEINLINE virtual void SetSteeringComponent(UG_SteeringComponent& _steeringComponent) override { mSteeringComponent = &_steeringComponent; }
	FORCEINLINE virtual void SetFloatingPawnComponent(UFloatingPawnMovement& _floatingPawnComponent)  override { mPawnMovementComponent = &_floatingPawnComponent; }

	FORCEINLINE float GetMobVelocity() const override { return mPawnMovementComponent ? mPawnMovementComponent->Velocity.Size() : 0; };
	FORCEINLINE G_ETypeMob GetTypeMob() const override { return mType; };

protected:
	AG_Boomer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SpawnAnimation() override;
	void ScaleSpawnAnimation();
	virtual void InitBrain() override;
	virtual void CreateAIComponents() override;
	virtual void InitMob() override;
	virtual void InitStatsFromDatatable(FG_EnemyData& _data) override;
	virtual void InitComponentsWithData(FG_EnemyData& _data) override;

#pragma region Override
public:
	virtual void SlowSpeed(const float& _speed) override;
	virtual void AddSpeed(const float& _speed) override;
#pragma endregion Override

	void GetBoomerHandler();
	void AddToBoomerHandler(const bool _add);
public:
	void InitSpawned(AG_BomberSpawner* _spawner);
	void BlockAttack(const bool& _block);
	void BlockOtherBoomers();
};