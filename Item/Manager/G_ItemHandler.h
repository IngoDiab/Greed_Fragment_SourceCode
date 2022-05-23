#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Greed/Item/Enum/G_ItemType.h"
#include "Greed/Item/Structures/G_WeaponItemInfo.h"
#include "Greed/Item/Structures/G_SkillItemInfo.h"
#include "Greed/Item/Structures/G_EffectItemInfo.h"
#include "Greed/Effects/Effect/EffectDT/G_EffectDT.h"
#include "Greed/Item/Manager/LevelItemManager/G_LevelItemManager.h"
#include "G_ItemHandler.generated.h"

class AG_InteractPick;
class UG_Effect;
class UG_Skill;
class AG_SkillItem;
class UG_Weapon;
class AG_WeaponItem;

UCLASS(Blueprintable, BlueprintType)
class GREED_API UG_ItemHandler : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<UG_LevelItemManager> mLevelItemManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "Datatable", meta = (DisplayName = "Weapons Datatable"))
	UDataTable* mWeaponDatatable = nullptr;
	UPROPERTY(EditAnywhere, Category = "Datatable", meta = (DisplayName = "Skills Datatable"))
	UDataTable* mSkillsDatatable = nullptr;
	UPROPERTY(EditAnywhere, Category = "Datatable", meta = (DisplayName = "Effects Datatable"))
	UDataTable* mEffectsDatatable = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Data", meta = (DisplayName = "Weapons Data"))
	TMap<uint8, FG_WeaponItemInfo> mWeaponsData;
	UPROPERTY(EditAnywhere, Category = "Data", meta = (DisplayName = "Skills Data"))
	TMap<uint8, FG_SkillItemInfo> mSkillsData;
	UPROPERTY(EditAnywhere, Category = "Data", meta = (DisplayName = "Effects Data"))
	TMap<uint8, FG_EffectItemInfo> mEffectsData;

public:
	FORCEINLINE void SetLevelItemManager(UG_LevelItemManager& _levelItemManager) { mLevelItemManager = &_levelItemManager; };

private:
	void InitItemManager(const UWorld& _world);

	void LoadDatatables();

	template <typename StructType>
	void Load(TMap<uint8, StructType>& _dataHandler, const UDataTable& _table);

	template <typename StructType>
	bool Contains(const uint8& _id, TMap<uint8, StructType> _handler) const;

	template <typename StructType>
	TArray<StructType*> GetDatas(const UDataTable& _table);
	

	void FillEffectItem(AG_InteractPick& _item, const TArray<UG_Effect*>& _effects) const;
	void FillEffectSkill(UG_Skill& _skill, const TArray<uint8>& _effectsID) const;
	void FillEffectWeapon(UG_Weapon& _weapon, const TArray<uint8>& _effectsID) const;
	TSubclassOf<UG_Effect> GetBPEEffect(const uint8 _id) const;
	void RandomizeSlotEffect(AG_WeaponItem& _weapon, uint8& _slot, TArray<uint8>& _allEffectsID) const;

public:
	virtual void PostInitProperties() override;

	FG_WeaponItemInfo GetWeaponInfoItem(const uint8& _id) const;
	FG_SkillItemInfo GetSkillInfoItem(const uint8& _id) const;
	FG_EffectItemInfo GetEffectInfoItem(const uint8& _id) const;

	UG_Weapon* ItemIntoWeapon(const uint8& _id, const uint8& _lvl, const TArray<uint8>& _effectsID);
	AG_WeaponItem* WeaponIntoItem(UWorld& _world, const uint8& _id, const uint8& _lvl, const TArray<UG_Effect*>& _effects, const FVector& _pos);
	UG_Skill* ItemIntoSkill(UWorld& _world, AActor& _ownerSkill, const uint8& _id, const uint8& _lvl, const TArray<uint8>& _effectsID);
	AG_SkillItem* SkillIntoItem(UWorld& _world, const uint8& _id, const uint8& _lvl, const TArray<UG_Effect*>& _effects, const FVector& _pos);

	void InitItemRandomly(AG_InteractPick& _interactPick, const uint8& _lvl) const;
	template<typename PickableType>
	PickableType* SpawnPickable(UWorld& _world, UClass& _pickableClass, const FVector& _pos, const FRotator& _rotation);
	template<typename PickableType>
	PickableType* SpawnPickableDeferred(UWorld& _world, UClass& _pickableClass, const FTransform& _transform);
	void RemovePickableFromLevelItemManager(AG_Pickable& _pickable) const;
};

template <typename StructType>
void UG_ItemHandler::Load(TMap<uint8, StructType>& _dataHandler, const UDataTable& _table)
{
	if (!&_dataHandler || !&_table) return;
	TArray<StructType*> _allEffects = GetDatas<StructType>(_table);
	for (uint8 _i = 0; _i < _allEffects.Num(); _i++)
		_dataHandler.Add(_allEffects[_i]->GetID(), *_allEffects[_i]);
}

template<typename StructType>
TArray<StructType*> UG_ItemHandler::GetDatas(const UDataTable& _table)
{
	TArray<StructType*> _allRows;
	_table.GetAllRows(FString(), _allRows);
	return _allRows;
}

template<typename StructType>
bool UG_ItemHandler::Contains(const uint8& _id, TMap<uint8, StructType> _handler) const
{
	for (TTuple<uint8, StructType> _pair : _handler)
		if (_pair.Key == _id) return true;
	return false;
}

template<typename PickableType>
PickableType* UG_ItemHandler::SpawnPickable(UWorld& _world, UClass& _pickableClass, const FVector& _pos, const FRotator& _rotation)
{
	if (!mLevelItemManager.IsValid()) InitItemManager(_world);
	if (!mLevelItemManager.IsValid()) return nullptr;
	AG_Pickable* _pickable = _world.SpawnActor<AG_Pickable>(&_pickableClass, _pos, FRotator::ZeroRotator);
	if (!_pickable) return nullptr;
	mLevelItemManager->AddPickable(*_pickable);
	return Cast<PickableType>(_pickable);
}

template<typename PickableType>
PickableType* UG_ItemHandler::SpawnPickableDeferred(UWorld& _world, UClass& _pickableClass, const FTransform& _transform)
{
	if (!mLevelItemManager.IsValid()) InitItemManager(_world);
	if (!mLevelItemManager.IsValid()) return nullptr;
	UE_LOG(LogTemp, Warning, TEXT("mLevelItemManager"));
	AG_Pickable* _pickable = _world.SpawnActorDeferred<AG_Pickable>(&_pickableClass, _transform);
	if (!_pickable) return nullptr;
	UE_LOG(LogTemp, Warning, TEXT("_pickable"));
	mLevelItemManager->AddPickable(*_pickable);
	return Cast<PickableType>(_pickable);
}
