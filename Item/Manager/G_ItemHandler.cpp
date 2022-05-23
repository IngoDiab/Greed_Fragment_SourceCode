#include "G_ItemHandler.h"

#include "Kismet/KismetMathLibrary.h"

#include "Greed/Manager/GameMode/G_GameMode.h"
#include "Greed/Skills/Skill/G_Skill.h"
#include "Greed/Weapons/Weapon/G_Weapon.h"
#include "Greed/Item/Item/Pickable/InteractPickable/SkillItem/G_SkillItem.h"
#include "Greed/Item/Item/Pickable/InteractPickable/WeaponItem/G_WeaponItem.h"
#include "Greed/Effects/Effect/G_Effect.h"

void UG_ItemHandler::PostInitProperties()
{
	Super::PostInitProperties();
	LoadDatatables();
}

void UG_ItemHandler::InitItemManager(const UWorld& _world)
{
	AG_GameMode* _gm = _world.GetAuthGameMode<AG_GameMode>();
	if (!_gm)return;
	mLevelItemManager = MakeWeakObjectPtr(_gm->LevelItemManager());
}

void UG_ItemHandler::LoadDatatables()
{
	Load<FG_WeaponItemInfo>(mWeaponsData, *mWeaponDatatable);
	Load<FG_SkillItemInfo>(mSkillsData, *mSkillsDatatable);
	Load<FG_EffectItemInfo>(mEffectsData, *mEffectsDatatable);
}

FG_WeaponItemInfo UG_ItemHandler::GetWeaponInfoItem(const uint8& _id) const
{
	if (!Contains<FG_WeaponItemInfo>(_id, mWeaponsData)) return FG_WeaponItemInfo();
	return mWeaponsData[_id];
}

FG_SkillItemInfo UG_ItemHandler::GetSkillInfoItem(const uint8& _id) const
{
	if (!Contains<FG_SkillItemInfo>(_id, mSkillsData)) return FG_SkillItemInfo();
	return mSkillsData[_id];
}

FG_EffectItemInfo UG_ItemHandler::GetEffectInfoItem(const uint8& _id) const
{
	if (!Contains<FG_EffectItemInfo>(_id, mEffectsData)) return FG_EffectItemInfo();
	return mEffectsData[_id];
}

UG_Weapon* UG_ItemHandler::ItemIntoWeapon(const uint8& _id, const uint8& _lvl, const TArray<uint8>& _effectsID)
{
	FG_WeaponItemInfo _weaponInfo;
	_weaponInfo = GetWeaponInfoItem(_id);
	UClass* _classToSpawn = _weaponInfo.GetBPWeapon();
	if (!_classToSpawn) return nullptr;
	UG_Weapon* _weapon = NewObject<UG_Weapon>(this, _classToSpawn);
	if (!_weapon) return nullptr;
	_weapon->SetLevel(_lvl);
	FillEffectWeapon(*_weapon, _effectsID);
	return _weapon;
}

AG_WeaponItem* UG_ItemHandler::WeaponIntoItem(UWorld& _world, const uint8& _id, const uint8& _lvl, const TArray<UG_Effect*>& _effects, const FVector& _pos)
{
	if (!&_world) return nullptr;
	FG_WeaponItemInfo _weaponInfo;
	_weaponInfo = GetWeaponInfoItem(_id);
	UClass* _classToSpawn = _weaponInfo.GetBPItem();

	if (!_classToSpawn) return nullptr;
	FTransform _transform = FTransform();
	_transform.SetLocation(_pos);
	AG_WeaponItem* _weaponItem = SpawnPickableDeferred<AG_WeaponItem>(_world, *_classToSpawn, _transform);

	if (!_weaponItem) return nullptr;
	_weaponItem->SetLevel(_lvl);
	FillEffectItem(*_weaponItem, _effects);
	_weaponItem->FinishSpawning(_transform);
	return _weaponItem;
}

UG_Skill* UG_ItemHandler::ItemIntoSkill(UWorld& _world, AActor& _ownerSkill, const uint8& _id, const uint8& _lvl, const TArray<uint8>& _effectsID)
{
	FG_SkillItemInfo _skillInfo;
	_skillInfo = GetSkillInfoItem(_id);
	UClass* _classToSpawn = _skillInfo.GetBPSkill();
	if (!_classToSpawn) return nullptr;
	UG_Skill* _skill = NewObject<UG_Skill>(this, _classToSpawn);
	if (!_skill) return nullptr;
	_skill->SetLevelSkill(_lvl);
	_skill->InitSkill(&_world, &_ownerSkill);
	FillEffectSkill(*_skill, _effectsID);
	return _skill;
}

AG_SkillItem* UG_ItemHandler::SkillIntoItem(UWorld& _world, const uint8& _id, const uint8& _lvl, const TArray<UG_Effect*>& _effects, const FVector& _pos)
{
	if (!&_world) return nullptr;
	FG_SkillItemInfo _skillInfo;
	_skillInfo = GetSkillInfoItem(_id);
	UClass* _classToSpawn = _skillInfo.GetBPItem();

	if (!_classToSpawn) return nullptr;
	FTransform _transform = FTransform();
	_transform.SetLocation(_pos);
	AG_SkillItem* _skillItem = SpawnPickableDeferred<AG_SkillItem>(_world, *_classToSpawn, _transform);

	if (!_skillItem) return nullptr;
	_skillItem->SetLevel(_lvl);
	FillEffectItem(*_skillItem, _effects);
	_skillItem->FinishSpawning(_transform);
	return _skillItem;
}

void UG_ItemHandler::RemovePickableFromLevelItemManager(AG_Pickable& _pickable) const
{
	if (!mLevelItemManager.IsValid())return;
	mLevelItemManager->RemovePickable(_pickable);
}

void UG_ItemHandler::FillEffectItem(AG_InteractPick& _item, const TArray<UG_Effect*>& _effects) const
{
	if (_effects.Num() <= 0) return;
	for (UG_Effect* _effect : _effects)
	{
		if (!_effect) continue;
		const uint8 _effectID = _effect->GetIDDatabase();
		if (_effectID == 0) continue;
		_item.AddEffectID(_effectID);
	}
}

void UG_ItemHandler::FillEffectSkill(UG_Skill& _skill, const TArray<uint8>& _effectsID) const
{
	if (!&_skill) return;
	if (_effectsID.Num() <= 0) return;
	for (uint8 _effectID : _effectsID)
	{
		if (_effectID == 0) continue;
		FEffectDT _dt = FEffectDT(FName(GetEffectInfoItem(_effectID).GetName()), GetBPEEffect(_effectID));
		_skill.AddEffect(_dt);
	}
}

void UG_ItemHandler::FillEffectWeapon(UG_Weapon& _weapon, const TArray<uint8>& _effectsID) const
{
	if (!&_weapon || _effectsID.Num() <= 0) return;
	TArray<FEffectDT> _effects = TArray<FEffectDT>();
	for (uint8 _effectID : _effectsID)
	{
		if (_effectID == 0) continue;
		FEffectDT _dt = FEffectDT(FName(GetEffectInfoItem(_effectID).GetName()), GetBPEEffect(_effectID));
		_dt.SetTypeWeapon((int)_weapon.GetTypeWeapon());
		_effects.Add(_dt);
	}
	_weapon.SetEffects(_effects);
}

TSubclassOf<UG_Effect> UG_ItemHandler::GetBPEEffect(const uint8 _id) const
{
	FG_EffectItemInfo _effectInfo;
	_effectInfo = GetEffectInfoItem(_id);
	return _effectInfo.GetBPEffect();
}

void UG_ItemHandler::InitItemRandomly(AG_InteractPick& _interactPick, const uint8& _lvl) const
{
	_interactPick.SetLevel(_lvl);
	if (_interactPick.GetType() != G_ItemType::WEAPON || mEffectsData.Num() <= 1) return;
	AG_WeaponItem* _weaponItem = Cast<AG_WeaponItem>(&_interactPick);
	if (!_weaponItem)return;
	TArray<uint8> _allEffectsID = TArray<uint8>();
	uint8 _i = 1;
	while (_i <= _lvl && _i <= mEffectsData.Num()-1) RandomizeSlotEffect(*_weaponItem, _i, _allEffectsID);
}

void UG_ItemHandler::RandomizeSlotEffect(AG_WeaponItem& _weapon, uint8& _slot, TArray<uint8>& _allEffectsID) const
{
	const uint8 _randomID = UKismetMathLibrary::RandomIntegerInRange(1, mEffectsData.Num() - 1);
	if (_allEffectsID.Contains(_randomID)) return;
	_weapon.AddEffectID(_randomID);
	_allEffectsID.Add(_randomID);
	_slot++;
}
