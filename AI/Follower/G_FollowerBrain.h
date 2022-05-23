#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/Brain/G_Brain.h"
#include "G_FollowerBrain.generated.h"

UCLASS()
class GREED_API UG_FollowerBrain : public UG_Brain
{
private:
	GENERATED_BODY()
	
#pragma region f/p	
	UPROPERTY(Category = "Brain | GD", EditAnywhere, meta = (DisplayName = "reload time"))
	float mReloadTime = 15;
	UPROPERTY(Category = "Brain | GD", EditAnywhere, meta = (DisplayName = "wait while reload"))
	bool mWaitWhileReload = false;
#pragma endregion

#pragma region Override
	protected:
	virtual void Update(const float _deltaTime) override;
	virtual void InitFSM() override;

#pragma endregion Override
};
