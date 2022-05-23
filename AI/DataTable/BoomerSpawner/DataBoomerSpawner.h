#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/DataTable/G_EnemyData.h"
#include "DataBoomerSpawner.generated.h"

USTRUCT()
struct GREED_API FDataBoomerSpawner : public FG_EnemyData
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Spawn delay"))
	float mDelayForSpawn = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Spawn radius"))
	float mRadiusSpawner = 300;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Max boomer(s) spawned at the same time"))
	int mMaxNbBoomer = 3;

public:
	FORCEINLINE float GetDelay() const { return mDelayForSpawn; };
	FORCEINLINE float GetRadiusSpawner() const { return mRadiusSpawner; };
	FORCEINLINE float GetMaxNbBoomer() const { return mMaxNbBoomer; };
};
