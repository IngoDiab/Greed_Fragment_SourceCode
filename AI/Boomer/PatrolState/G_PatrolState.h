#pragma once

#include "CoreMinimal.h"
#include "Greed/FSM/FSMState/G_FSMState.h"
#include "G_PatrolState.generated.h"

class UG_MovementComponent;

UCLASS()
class GREED_API UG_PatrolState : public UG_FSMState
{

	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	UG_MovementComponent* mMovementComponent = nullptr;

	UPROPERTY(Category="Patrol State | Settings", VisibleAnywhere, meta = (DisplayName = "Patrol Range"))
	float mPatrolRange = 100;

	UPROPERTY(Category="Patrol State | Settings", VisibleAnywhere, meta = (DisplayName = "Spawn Location"))
	FVector mSpawnLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle mHandle;

private:
	void GetUsefulRef();
	void GetNewPatrolPoint() const;
	void UpdateMovement();

	void DelegateMove();
	void EndDelegateMove();

public:
	virtual void InitState(UG_Brain* _brain) override;
	virtual void BeginDestroy() override;
};
