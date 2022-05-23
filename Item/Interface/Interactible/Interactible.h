#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Greed/Utils/Action/Action.h"
#include "Interactible.generated.h"

UINTERFACE(MinimalAPI)
class UInteractible : public UInterface
{
	GENERATED_BODY()
};

class GREED_API IInteractible
{
	GENERATED_BODY()

public:
	virtual void EnableUI(const bool& _enable) PURE_VIRTUAL(IInteractible::EnableUI, ;);
	virtual FVector GetInteractionLocation() const PURE_VIRTUAL(IInteractible::GetInteractionLocation, return FVector::ZeroVector;);
	virtual void Interact() PURE_VIRTUAL(IInteractible::Interact, ;);
	virtual bool IsInteractible() const PURE_VIRTUAL(IInteractible::IsInteractible, return true;);
	virtual Action<>* OnInteract() PURE_VIRTUAL(IInteractible::OnInteract, return nullptr;);
};
