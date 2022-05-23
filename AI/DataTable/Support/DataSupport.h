#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/DataTable/Mob/DataMob.h"
#include "Greed/Weapons/Weapon/G_Weapon.h"
#include "Greed/AI/Artificier/FleeState/FleeStruct/G_FleeStruct.h"
#include "DataSupport.generated.h"

USTRUCT()
struct GREED_API FDataSupport : public FDataMob
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
		TMap<ETypeWeapon, FG_FleeStruct> mFleeDatas = TMap<ETypeWeapon, FG_FleeStruct>();

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Angle To Dodge Next Shot"))
		float mAngleToDodge = 60;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Distance To Dodge Next Shot"))
		float mDistanceToDodge = 400;

public:
	FORCEINLINE TMap<ETypeWeapon, FG_FleeStruct> GetFleeDatas() const { return mFleeDatas; };
	FORCEINLINE float GetAngleToDodge() const { return mAngleToDodge; };
	FORCEINLINE float GetDistanceToDodge() const { return mDistanceToDodge; };
};
