#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Greed/AI/FX/G_FXData.h"
#include "Greed/AI/Enum/G_ETypeMob.h"
#include "G_FXManager.generated.h"

USTRUCT()
struct GREED_API FFXByTypeMob
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "FXByType"))
	TMap<FString, FG_FXData> mFXByType = TMap<FString, FG_FXData>();

public:
	FORCEINLINE FG_FXData GetFXData(const FString& _type) const { return mFXByType.Contains(_type) ? mFXByType[_type] : FG_FXData(); };
};

USTRUCT()
struct GREED_API FFXByLevelLoot
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "FXByLevel"))
		TArray<FG_FXData> mFXByLevel = TArray<FG_FXData>();

public:
	FORCEINLINE FG_FXData GetFXData(const uint8& _level) const { return _level >= 0 && _level < mFXByLevel.Num() ? mFXByLevel[_level] : FG_FXData(); };
};

UCLASS(Blueprintable)
class GREED_API UG_FXManager : public UObject
{
	GENERATED_BODY()

	UPROPERTY(Category = "Data", EditAnywhere, meta = (DisplayName = "Mobs FX"))
	TMap<G_ETypeMob, FFXByTypeMob> mFXMob = TMap<G_ETypeMob, FFXByTypeMob>();
	
	UPROPERTY(Category = "Data", EditAnywhere, meta = (DisplayName = "Loot FX"))
	TMap<FString, FFXByLevelLoot> mFXLoot = TMap<FString, FFXByLevelLoot>();

public:
	void SpawnFXMob(const G_ETypeMob& _typeMob, const FString& _typeFX, const FVector& _position, const FRotator& _rotation);
	void SpawnFXLoot(const FString& _typeFX, const uint8& _levelLoot, const FVector& _position, const FRotator& _rotation);
	void SpawnFXLoot(const FString& _typeFX, const uint8& _levelLoot, UNiagaraComponent& _fxComp);
};
