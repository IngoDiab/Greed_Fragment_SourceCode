#pragma once

#include "CoreMinimal.h"
#include "G_ItemInfo.h"
#include "Greed\Effects\Effect\G_Effect.h"
#include "G_EffectItemInfo.generated.h"

USTRUCT()
struct GREED_API FG_EffectItemInfo : public FG_ItemInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "BP", meta = (DisplayName = "BP Effect"))
		TSubclassOf<UG_Effect> mBPEffect = nullptr;

public:
	FORCEINLINE UClass* GetBPEffect() const { return mBPEffect.Get(); };
	
};
