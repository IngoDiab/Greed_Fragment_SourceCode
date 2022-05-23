#pragma once

#include "CoreMinimal.h"
#include "G_FleeStruct.generated.h"

USTRUCT()
struct GREED_API FG_FleeStruct
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Distance min between target and mob"))
		float mDistanceMinBeforeFlee = 200;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Distance to flee from target"))
		float mDistanceToFlee = 700;

public:
	FORCEINLINE float DistanceMinBeforeFlee() const { return mDistanceMinBeforeFlee; };
	FORCEINLINE float DistanceToFlee() const { return mDistanceToFlee; };	
};
