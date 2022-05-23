#pragma once

#include "CoreMinimal.h"
#include "Greed/Skills/Skill/G_Skill.h"
#include "G_Phase.generated.h"

USTRUCT()
struct GREED_API FG_Phase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Need HP changes to end phase"))
		bool mNeedHPToEndPhase = true;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Percent HP Before End Phase", ClampMin = "0", ClampMax = "100", EditCondition = "mNeedHPToEndPhase", EditConditionHides))
		uint8 mPercentHPToEndPhase = 0;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Transition Delay"))
		float mTransitionDelay = .5f;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Skills for this phase"))
		TArray<TSubclassOf<UG_Skill>> mSkillsForPhase = TArray<TSubclassOf<UG_Skill>>();

public:
	FORCEINLINE bool NeedHPToEndPhase() const { return mNeedHPToEndPhase; };
	FORCEINLINE uint8 GetPercentHPToEndPhase() const { return mPercentHPToEndPhase; };
	FORCEINLINE float GetTransitionDelay() const { return mTransitionDelay; };
	FORCEINLINE TArray<TSubclassOf<UG_Skill>> GetSkillsForPhase() const { return mSkillsForPhase; };
};
