#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "G_LevelItemManager.generated.h"

class AG_Pickable;

UCLASS()
class GREED_API UG_LevelItemManager : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Manager", meta = (DisplayName = "Pickables in Scene"))
	TArray<AG_Pickable*> mAllPickablesInScene = TArray<AG_Pickable*>();

public:
	UG_LevelItemManager();
	void AddPickable(AG_Pickable& _pickable);
	void RemovePickable(AG_Pickable& _pickable);

private:
	void DestroyPickables();
};
