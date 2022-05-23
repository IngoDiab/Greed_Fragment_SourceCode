#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/G_BaseAiComponent.h"
#include "G_SupportPositioningComponent.generated.h"

UCLASS()
class GREED_API UG_SupportPositioningComponent : public UG_BaseAiComponent
{
#pragma region f/p
	
	GENERATED_BODY()
	public:
		Action<FVector> FollowMob;
	private:
	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "All mob in range"))
	TArray<AG_BaseMob*> mAllMob;

	private:
	UPROPERTY()
	int mCurrentHealerWeight = 0;
#pragma endregion f/p

#pragma region UE
	protected:
	virtual void BeginPlay() override;
#pragma endregion UE

#pragma region Custom
	public:
	UFUNCTION()
    virtual void InRange(AActor* _thisActor, AActor* _otherActor);
	UFUNCTION()
    virtual void OutRange(AActor* _thisActor, AActor* _otherActor);
#pragma endregion  Custom


};
