#pragma once

#include "CoreMinimal.h"
#include "Greed/FSM/FSMState/G_FSMState.h"
#include "G_FleeState.generated.h"

class UG_MovementComponent;
class UG_FleeComponent;

UCLASS()
class GREED_API UG_FleeState : public UG_FSMState
{
private:
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UG_MovementComponent* mMovementComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UG_FleeComponent* mFleeComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	FTimerHandle mHandle;

private:
	void GetUsefulRef();
	void Flee();

	void DelegateMove();
	void EndDelegateMove();

public:
	virtual void InitState(UG_Brain* _brain) override;
	virtual void BeginDestroy() override;
};
