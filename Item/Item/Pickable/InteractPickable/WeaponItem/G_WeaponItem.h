#pragma once

#include "CoreMinimal.h"
#include "Greed\Item\Item\Pickable\InteractPickable\G_InteractPick.h"
#include "G_WeaponItem.generated.h"

class UG_UI_WeaponInfo;

UCLASS()
class GREED_API AG_WeaponItem : public AG_InteractPick
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UG_UI_WeaponInfo* mWeaponInfo = nullptr;

protected:
	/// <summary>
	/// Init Events
	/// </summary>
	virtual void InitEvents() override;

public:
	void EquipWeapon();

protected:
	virtual void InitPickable() override;
	virtual void InitUI();
	void GetMeshLevel();
};
