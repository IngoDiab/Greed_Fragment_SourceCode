#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "G_RepulsionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREED_API UG_RepulsionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UG_RepulsionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
