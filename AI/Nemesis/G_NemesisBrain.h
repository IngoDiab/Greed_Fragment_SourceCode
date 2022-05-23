#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/Brain/G_Brain.h"
#include "Greed/AI/Nemesis/Phase/G_Phase.h"
#include "G_NemesisBrain.generated.h"

UCLASS()
class GREED_API UG_NemesisBrain : public UG_Brain
{
	GENERATED_BODY()
	
protected:
	virtual void Update(const float _deltaTime) override;
	virtual void CreateFightListener() override;


public:
	void FillSkillPhase(const FG_Phase& _phase);
	void FinishSkills();
};
