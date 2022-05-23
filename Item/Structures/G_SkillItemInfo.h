#pragma once

#include "CoreMinimal.h"
#include "G_ItemInfo.h"
#include "Greed\Item\Item\Pickable\InteractPickable\G_InteractPick.h"
#include "Greed\Skills\Skill\G_Skill.h"
#include "G_SkillItemInfo.generated.h"

USTRUCT()
struct GREED_API FG_SkillItemInfo : public FG_ItemInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "BP", meta = (DisplayName = "BP Skill"))
	TSubclassOf<UG_Skill> mBPSkill = nullptr;

	UPROPERTY(EditAnywhere, Category = "BP", meta = (DisplayName = "BP Item"))
	TSubclassOf<AG_InteractPick> mBPItem = nullptr;

public:
	FORCEINLINE UClass* GetBPSkill() const { return mBPSkill.Get(); };
	FORCEINLINE UClass* GetBPItem() const { return mBPItem.Get(); };
};
