#include "G_WeaponItem.h"

#include "Greed/Item/Manager/G_ItemHandler.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"
#include "Greed/UI/Components/G_UI_ComponentBase.h"
#include "Greed/UI/Info/G_UI_WeaponInfo.h"

void AG_WeaponItem::InitEvents()
{
	Super::InitEvents();
	mOnInteract += [this]()
	{
		EquipWeapon();
	};

	mOnUpdatePickable += [this](float DeltaTime)
	{
		Rotate(DeltaTime, *mMesh);
	};
}

void AG_WeaponItem::EquipWeapon()
{
	if (!mPlayer || !mPlayerInterComp || !mItemHandler) return;
	const FVector _pickedUpPos = GetActorLocation();
	UG_Weapon* _pickedUpWeapon = mItemHandler->ItemIntoWeapon(mID, mLevel, mEffectsID);

	mPlayer->PickUpWeapon(_pickedUpWeapon, _pickedUpPos);
	mPlayerInterComp->RemoveFromAllInteractible(this);
	mItemHandler->RemovePickableFromLevelItemManager(*this);
	BeginDestroyItem();
}

void AG_WeaponItem::InitPickable()
{
	InitUI();
	Super::InitPickable();
	GetMeshLevel();
	mOnLevelChanged += ([this] { GetMeshLevel(); });
}

void AG_WeaponItem::InitUI()
{
	if (!mUIComponent)return;
	mWeaponInfo = Cast<UG_UI_WeaponInfo>(mUIComponent->GetWidget());
	if (!mWeaponInfo) return;
	mWeaponInfo->Init(this);
}

void AG_WeaponItem::GetMeshLevel()
{
	if (!mMesh || !mItemHandler) return;
	FG_WeaponItemInfo _infoWeapon = mItemHandler->GetWeaponInfoItem(mID);
	UStaticMesh* _mesh = _infoWeapon.GetMesh(mLevel);
	if (!_mesh) return;
	mMesh->SetStaticMesh(_mesh);
}