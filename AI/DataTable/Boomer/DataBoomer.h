#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/DataTable/Mob/DataMob.h"
#include "DataBoomer.generated.h"

USTRUCT()
struct GREED_API FDataBoomer : public FDataMob
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Detection Range"))
	float mDetectionRange = 120;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Patrol Range from Spawn"))
	float mPatrolRange = 120;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Min time to wait at a Patrol point"))
	float mTimeWaitMin = 120;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Max time to wait at a Patrol point"))
	float mTimeWaitMax = 120;

public:
	FORCEINLINE float GetDetectionRange() const { return mDetectionRange; };
	FORCEINLINE float GetPatrolRange() const { return mPatrolRange; };
	FORCEINLINE float GetTimeWaitMin() const { return mTimeWaitMin; };
	FORCEINLINE float GetTimeWaitMax() const { return mTimeWaitMin; };
};
