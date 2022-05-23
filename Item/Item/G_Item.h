#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Greed/Item/Interface/HandledItemDatabase/HandledItemDatabase.h"
#include "G_Item.generated.h"

UCLASS(ABSTRACT)
class GREED_API AG_Item : public AActor, public IHandledItemDatabase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Item", meta = (DisplayName = "Item ID"))
	uint8 mID = 0;

public:
	virtual uint8 GetIDDatabase() const override;
	virtual void SetIDDatabase(const uint8& _id) override;
};
