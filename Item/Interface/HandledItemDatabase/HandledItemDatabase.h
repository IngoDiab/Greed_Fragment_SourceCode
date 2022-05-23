#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/DataTable.h"
#include "HandledItemDatabase.generated.h"

UINTERFACE(MinimalAPI)
class UHandledItemDatabase : public UInterface
{
	GENERATED_BODY()
};

class GREED_API IHandledItemDatabase
{
	GENERATED_BODY()

public:
	virtual uint8 GetIDDatabase() const PURE_VIRTUAL(IHandledItemDatabase::GetIDDatabase, return 0;);
	virtual void SetIDDatabase(const uint8& _id) PURE_VIRTUAL(IHandledItemDatabase::SetIDDatabase, ;);
};
