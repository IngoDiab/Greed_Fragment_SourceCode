#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"
#include "Greed/AI/Component/G_FleeComponent.h"
#include "FleeingMob.generated.h"

UINTERFACE(MinimalAPI)
class UFleeingMob : public UMovingMob
{
	GENERATED_BODY()
};

class GREED_API IFleeingMob : public IMovingMob
{
	GENERATED_BODY()

public:
	virtual UG_FleeComponent* GetFleeComponent() PURE_VIRTUAL(IFleeingMob::GetFleeComponent, return nullptr;);
	virtual void SetFleeComponent(UG_FleeComponent& _fleeComponent) PURE_VIRTUAL(IFleeingMob::SetFleeComponent, ;);
};
