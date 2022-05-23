#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "G_ItemInfo.generated.h"

USTRUCT()
struct GREED_API FG_ItemInfo : public FTableRowBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Infos", meta = (DisplayName = "ID"))
	uint8 mID = 0;

	UPROPERTY(EditAnywhere, Category = "Infos", meta = (DisplayName = "Icone"))
	TArray<UTexture2D*> mAllIcone;
	
	UPROPERTY(EditAnywhere, Category = "Infos", meta = (DisplayName = "Name"))
	FString mName = "";
	
	UPROPERTY(EditAnywhere, Category = "Infos", meta = (DisplayName = "Description"))
	FString mDesc = "";

public:
	FORCEINLINE uint8 GetID() const { return mID; }
	FORCEINLINE UTexture2D* GetIcone(const uint8 _lvl)
	{
		const uint8 _nbIcon = mAllIcone.Num();
		if (_nbIcon <= 0) return nullptr;
		if (_nbIcon < _lvl) return mAllIcone[_nbIcon - 1];
		return mAllIcone[_lvl];
	}
	FORCEINLINE FString GetName() const { return mName; }
	FORCEINLINE FString GetDesc() const { return mDesc; }
};
