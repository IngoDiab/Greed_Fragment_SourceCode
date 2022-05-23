#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Greed/AI/Enum/G_ETypeMob.h"
#include "Greed/AI/Component/G_MovementComponent.h"
#include "Greed/AI/Component/Steering/G_SteeringComponent.h"
#include "MovingMob.generated.h"

UINTERFACE(MinimalAPI)
class UMovingMob : public UInterface
{
	GENERATED_BODY()
};

class GREED_API IMovingMob
{
	GENERATED_BODY()

public:
	virtual UG_MovementComponent* GetMovementComponent() PURE_VIRTUAL(IMovingMob::GetMovementComponent, return nullptr;);
	virtual UG_SteeringComponent* GetSteeringComponent() PURE_VIRTUAL(IMovingMob::GetSteeringComponent, return nullptr;);
	virtual UFloatingPawnMovement* GetFloatingPawnComponent() PURE_VIRTUAL(IMovingMob::GetFloatingPawnComponent, return nullptr;);

	virtual void SetMovementComponent(UG_MovementComponent& _moveComponent) PURE_VIRTUAL(IMovingMob::SetMovementComponent, ;);
	virtual void SetSteeringComponent(UG_SteeringComponent& _steeringComponent) PURE_VIRTUAL(IMovingMob::SetSteeringComponent, ;);
	virtual void SetFloatingPawnComponent(UFloatingPawnMovement& _floatingPawnComponent) PURE_VIRTUAL(IMovingMob::SetFloatingPawnComponent, ;);

	virtual float GetMobVelocity() const PURE_VIRTUAL(IMovingMob::GetMobVelocity, return 0;);
	virtual G_ETypeMob GetTypeMob() const PURE_VIRTUAL(IMovingMob::GetTypeMob, return G_ETypeMob::OTHER;);
};
