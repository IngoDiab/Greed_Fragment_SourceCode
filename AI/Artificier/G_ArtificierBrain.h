#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/Brain/AnimatedBrain/G_AnimatedBrain.h"
#include "Greed/AI/DataTable/Artificier/DataArtificier.h"
#include "Greed/AI/Interfaces/FleeingMob/FleeingMob.h"
#include "G_ArtificierBrain.generated.h"

class UG_FleeComponent;

UCLASS()
class GREED_API UG_ArtificierBrain : public UG_AnimatedBrain, public IFleeingMob
{	
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Flee Component"))
	UG_FleeComponent* mFleeComponent = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Movement Component"))
	UG_MovementComponent* mMovementSystem = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Steering Component"))
	UG_SteeringComponent* mSteeringComponent = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", VisibleAnywhere, meta = (DisplayName = "Floating Pawn Component"))
	UFloatingPawnMovement* mPawnMovementComponent = nullptr;
	
	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere,BlueprintReadOnly)
	bool mNeedToFlee = false;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mRepositionAfterHit = false;
	
	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere,BlueprintReadOnly)
	bool mPlayerTooClose = false;

	UPROPERTY(Category = "Brain", VisibleAnywhere, meta = (DisplayName = "Can Relocate Steering"))
	bool mCanRelocateSteering = true;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere)
	bool mRelocateSteering = false;

public:
	FORCEINLINE virtual UG_MovementComponent* GetMovementComponent() override { return mMovementSystem; };
	FORCEINLINE virtual UG_SteeringComponent* GetSteeringComponent() override { return mSteeringComponent; };
	FORCEINLINE virtual UFloatingPawnMovement* GetFloatingPawnComponent() override { return mPawnMovementComponent; };
	FORCEINLINE virtual UG_FleeComponent* GetFleeComponent() override { return mFleeComponent; };

	FORCEINLINE virtual void SetMovementComponent(UG_MovementComponent& _moveComponent)  override { mMovementSystem = &_moveComponent; }
	FORCEINLINE virtual void SetSteeringComponent(UG_SteeringComponent& _steeringComponent) override { mSteeringComponent = &_steeringComponent; }
	FORCEINLINE virtual void SetFloatingPawnComponent(UFloatingPawnMovement& _floatingPawnComponent)  override { mPawnMovementComponent = &_floatingPawnComponent; }
	FORCEINLINE virtual void SetFleeComponent(UG_FleeComponent& _fleeComp) override { mFleeComponent = &_fleeComp; }

	FORCEINLINE float GetMobVelocity() const { return mPawnMovementComponent ? mPawnMovementComponent->Velocity.Size() : 0; };
	
protected:
	virtual void InitData(FDataMob& _data) override;
	virtual void CreateListener() override;
	virtual void CreateMovementListener();
	void CreateSteeringListener();
	virtual void CreateFleeListener();
	virtual void CreateFightListener() override;
	virtual void InitFSM() override;
	virtual void InitAnimations() override;
	virtual void Update(const float _deltaTime) override;

	virtual void UpdateBooleans() override;
	virtual void UpdateAnimations() override;
	
public:
	virtual bool IsValid() const override;
	virtual void Stun() override;
};