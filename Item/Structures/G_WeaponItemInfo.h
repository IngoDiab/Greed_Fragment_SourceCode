#pragma once

#include "CoreMinimal.h"
#include "G_ItemInfo.h"
#include "Greed\Item\Item\Pickable\InteractPickable\G_InteractPick.h"
#include "Greed\Weapons\Weapon\G_Weapon.h"
#include "G_WeaponItemInfo.generated.h"

USTRUCT()
struct GREED_API FG_WeaponItemInfo : public FG_ItemInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "BP", meta = (DisplayName = "BP Weapon"))
	TSubclassOf<UG_Weapon> mBPWeapon = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "BP", meta = (DisplayName = "BP Item"))
	TSubclassOf<AG_InteractPick> mBPItem = nullptr;

	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (DisplayName = "All Meshs"))
	TArray<UStaticMesh*> allMeshs;

public:
	FORCEINLINE UClass* GetBPWeapon() const { return mBPWeapon.Get(); };
	FORCEINLINE UClass* GetBPItem() const { return mBPItem.Get(); };
	FORCEINLINE UStaticMesh* GetMesh(const uint8& _lvl)
	{
		const uint8 _nbMesh = allMeshs.Num();
		if (_nbMesh <= 0) return nullptr;
		if (_nbMesh < _lvl) return allMeshs[_nbMesh-1];
		return allMeshs[_lvl];
	}
};
