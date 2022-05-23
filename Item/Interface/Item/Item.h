#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item.generated.h"

UINTERFACE(MinimalAPI)
class UItem : public UInterface
{
	GENERATED_BODY()
};

class GREED_API IItem
{
	GENERATED_BODY()

public:
	virtual uint8 GetID() const PURE_VIRTUAL(IItem::GetID, return 0;);
	virtual void SetID(const uint8& _id) PURE_VIRTUAL(IItem::SetID, ;);
};
