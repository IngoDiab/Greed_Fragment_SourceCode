#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/DataTable/G_EnemyData.h"
#include "Greed/Weapons/Weapon/G_Weapon.h"
#include "Greed/AI/Nemesis/Phase/G_Phase.h"
#include "DataNemesis.generated.h"

USTRUCT()
struct GREED_API FDataNemesis : public FG_EnemyData
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Use Life Per Weapon"))
	bool mUseLifePerWeapon = true;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Life Per Weapon", EditCondition = "mUseLifePerWeapon", EditConditionHides))
	TMap<ETypeWeapon, int> mLifePerWeapon = TMap<ETypeWeapon, int>();

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Rotation Speed"))
	float mRotationSpeed = 500;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = " Max Angle Between Target & BasicForward"))
	float mMaxAngleTargetBasicForward = 45;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "All Phases"))
	TArray<FG_Phase> mAllPhases = TArray<FG_Phase>();

public:
	FORCEINLINE bool UseLifePerWeapon() const { return mUseLifePerWeapon; };
	FORCEINLINE float GetLifePerWeapon(const ETypeWeapon& _typeWeapon) const { return mLifePerWeapon.Contains(_typeWeapon) ? mLifePerWeapon[_typeWeapon] : mMaxLife; };
	FORCEINLINE float GetRotationSpeed() const { return mRotationSpeed; };
	FORCEINLINE float GetMaxAngleTargetBasicForward() const { return mMaxAngleTargetBasicForward; };
	FORCEINLINE TArray<FG_Phase> GetAllPhases() const { return mAllPhases; };
};
