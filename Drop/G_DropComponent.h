#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Greed/Utils/Action/Action.h"
#include "G_DropComponent.generated.h"

class UG_ItemHandler;
class AG_Soul;
class AG_InteractPick;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREED_API UG_DropComponent : public UActorComponent
{
	GENERATED_BODY()

	Action<> mOnUpdateEvenInEditor = Action<>();
	Action<> mOnUpdate = Action<>();
	
#pragma region ExternsRef
	UPROPERTY(VisibleAnywhere, Category = "Externs References", meta = (DisplayName = "Item Handler"))
	UG_ItemHandler* mItemHandler = nullptr;
#pragma endregion

#pragma region Drop
	UPROPERTY(EditAnywhere, Category = "Drop", meta = (DisplayName = "Drop Rate", ClampMin = "0", ClampMax = "100"))
	float mDropRateSouls = 100;

	UPROPERTY(EditAnywhere, Category = "Drop", meta = (DisplayName = "Max Range Drop"))
	float mMaxRange = 100;
#pragma endregion

#pragma region Souls
	UPROPERTY(EditAnywhere, Category = "Souls", meta = (DisplayName = "Drop Souls"))
	bool mDropSouls = true;
	
	UPROPERTY(EditAnywhere, Category = "Souls", meta = (DisplayName = "Souls BP", EditCondition = "mDropSouls", EditConditionHides))
	TSubclassOf<AG_Soul> mSoulBP = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Souls", meta = (DisplayName = "Min Number Souls Droppable", EditCondition = "mDropSouls", EditConditionHides))
	uint8 mNbMinSouls = 1;
	
	UPROPERTY(EditAnywhere, Category = "Souls", meta = (DisplayName = "Max Number Souls Droppable", EditCondition = "mDropSouls", EditConditionHides))
	uint8 mNbMaxSouls = 4;
#pragma endregion

#pragma region Items
	UPROPERTY(EditAnywhere, Category = "Items", meta = (DisplayName = "Drop Items"))
	bool mDropItems = false;
	
	UPROPERTY(EditAnywhere, Category = "Items", meta = (DisplayName = "Droppable Items BP", EditCondition = "mDropItems", EditConditionHides))
	TArray<TSubclassOf<AG_InteractPick>> mItems;

	UPROPERTY(EditAnywhere, Category = "Items", meta = (DisplayName = "Number Items Droppable", EditCondition = "mDropItems", EditConditionHides))
	uint8 mNbItems = 1;
#pragma endregion

#pragma region Debugs
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "Use Debug"))
	bool mUseDebug = false;
	
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "Min Range Color Debug", EditCondition = "mUseDebug", EditConditionHides))
	FColor mColorMinRange = FColor::Red;

	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "Max Range Color Debug", EditCondition = "mUseDebug", EditConditionHides))
	FColor mColorMaxRange = FColor::White;
	
	UPROPERTY()
	bool mInGame = false;
#pragma endregion

public:
	FORCEINLINE bool GetCanDropSouls() const { return mDropSouls; };
	FORCEINLINE void SetCanDropSouls(const bool& _canDrop) { mDropSouls = _canDrop; };
	FORCEINLINE void SetDropRateSouls(const float& _dropRate) { mDropRateSouls = _dropRate; };
	FORCEINLINE void AddDropRateSouls(const float& _dropRate) { mDropRateSouls += _dropRate; };

	FORCEINLINE void SetItems(TArray<TSubclassOf<AG_InteractPick>> _mItems) {
		mDropItems = true;
		mItems = _mItems; }

public:	
	UG_DropComponent();
	virtual void PostInitProperties() override;

protected:
	virtual void BeginPlay() override;
	void InitItemHandler();
	virtual void BeginDestroy() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void SpawnObjectInRange(UWorld& _world, UClass& _classToSpawn, const FVector& _posSpawn, const FVector& _baseVector);
	void SpawnObjectInRange(UWorld& _world, UClass& _classToSpawn, const uint8& _level, const FVector& _posSpawn, const FVector& _baseVector);
	UClass* GetRandomItem();

public:
	void DropSouls(const FVector& _posSpawn, const FVector& _vectorToDropOn, const bool& _onlyOnThisVector);
	void DropItems(const FVector& _posSpawn, const FVector& _vectorToDropOn, const bool& _onlyOnThisVector, const uint8& _level);
	void DrawDebug();
};
