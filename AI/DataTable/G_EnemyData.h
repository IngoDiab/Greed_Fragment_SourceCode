#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "G_EnemyData.generated.h"

USTRUCT()
struct GREED_API FG_EnemyData : public FTableRowBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Max Life"))
	int mMaxLife = 120;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Max Armor"))
	int mMaxArmor = 0;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Armor at Spawn"))
	int mArmorAtSpawn = 0;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Support Weight"))
	uint8 mSupportWeight = 1;

public:
	FORCEINLINE int GetMaxLife() const { return mMaxLife; };
	FORCEINLINE int GetMaxArmor() const { return mMaxArmor; };
	FORCEINLINE int GetArmorAtSpawn() const { return mArmorAtSpawn; };
	FORCEINLINE uint8 GetSupportWeight() const { return mSupportWeight; };
};
