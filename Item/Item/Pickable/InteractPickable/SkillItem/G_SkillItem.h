#pragma once

#include "CoreMinimal.h"
#include "Greed\Item\Item\Pickable\InteractPickable\G_InteractPick.h"
#include "G_SkillItem.generated.h"

class AG_InteractPick;
class UG_SkillComponent;

UCLASS()
class GREED_API AG_SkillItem : public AG_InteractPick
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* mTargetView = nullptr;

private:
	void CallAchievements();

protected:
	UPROPERTY()
	UG_SkillComponent* mPlayerSkillComponent = nullptr;

protected:
	/// <summary>
	/// Init Events
	/// </summary>
	virtual void InitEvents() override;
	/// <summary>
	/// Init Possess Player
	/// </summary>
	virtual void InitPickable() override;
	/// <summary>
	/// Init View Target
	/// </summary>
	void InitViewTarget();
	/// <summary>
	/// Look at an Actor
	/// </summary>
	/// <param name="_actor">: Actor to look at</param>
	/// <param name="_compToRotate">: Component which must look to _actor</param>
	void LookAt(const AActor& _actor, USceneComponent& _compToRotate);

	void TryEquipSkill();

public:
	void EquipSkill(const int& _slotIndex);
};
