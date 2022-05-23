#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "G_AIController.generated.h"

UCLASS()
class GREED_API AG_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AG_AIController(const FObjectInitializer& ObjectInitializer);

private:
	virtual void OnPossess(APawn* InPawn) override;
	void InitSteering();
};
