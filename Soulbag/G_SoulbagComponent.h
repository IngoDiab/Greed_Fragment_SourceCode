#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Greed/Utils/Action/Action.h"
#include "G_SoulbagComponent.generated.h"

class UG_Spiritbag;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREED_API UG_SoulbagComponent : public UActorComponent
{
	GENERATED_BODY()

	Action<int> mOnAmountUpdated = Action<int>();
	Action<int> mOnIncreaseAmount = Action<int>();
	Action<int> mOnDecreaseAmount = Action<int>();
	Action<int> mOnResetAmount = Action<int>();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Amount"))
		int mSoulsAmount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UG_Spiritbag* mSpiritbag = nullptr;

public:
	FORCEINLINE Action<int>& OnAmountUpdated() { return mOnAmountUpdated; };
	FORCEINLINE Action<int>& OnIncreaseAmount() { return mOnIncreaseAmount; };
	FORCEINLINE Action<int>& OnDecreaseAmount() { return mOnDecreaseAmount; };
	FORCEINLINE Action<int>& OnResetAmount() { return mOnResetAmount; };

	FORCEINLINE int GetSoulsAmount() const { return mSoulsAmount; };
	FORCEINLINE UG_Spiritbag* GetSpiritbag() { return mSpiritbag; };

public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	void Increase(const int& _amount = 1);
	void Decrease(const int& _amount = 1);
	void Reset();		
	void ConvertToSpirits();
};
