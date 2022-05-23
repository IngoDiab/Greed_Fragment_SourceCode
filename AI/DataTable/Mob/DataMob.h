#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/DataTable/G_EnemyData.h"
#include "DataMob.generated.h"

USTRUCT()
struct GREED_API FDataMob : public FG_EnemyData
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Max Speed"))
		float mMaxSpeed = 120;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Speed at spawn"))
		float mSpeedAtSpawn = 120;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Rotation Speed"))
		float mRotationSpeed = 500;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Min Attack Range"))
		float mMinAttackRange = 120;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Max Attack Range"))
		float mMaxAttackRange = 150;

public:
	FORCEINLINE float GetMaxSpeed() const { return mMaxSpeed; };
	FORCEINLINE float GetSpeedAtSpawn() const { return mSpeedAtSpawn; };
	FORCEINLINE float GetRotationSpeed() const { return mRotationSpeed; };
	FORCEINLINE float GetMinAttackRange() const { return mMinAttackRange; };
	FORCEINLINE float GetMaxAttackRange() const { return mMaxAttackRange; };

};
