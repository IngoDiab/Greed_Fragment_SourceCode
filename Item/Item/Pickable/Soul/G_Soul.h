#pragma once

#include "CoreMinimal.h"
#include "../G_Pickable.h"
#include "G_Soul.generated.h"

class UNiagaraComponent;

UCLASS()
class GREED_API AG_Soul : public AG_Pickable
{
	GENERATED_BODY()
		
	Action<> mOnRangeAcquisition = Action<>();

	UPROPERTY(EditAnywhere, Category = "Components", meta = (DisplayName = "Niagara Component"))
		UNiagaraComponent* mNiagaraComponent = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Range", meta = (DisplayName = "Range Acquisition"))
		float mRangeAdd = 0;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Timer Dropping"))
		float mTimerDropping = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		bool mIsDropping = true;

	UPROPERTY(VisibleAnywhere, Category = "Movement", meta = (DisplayName = "Can Move"))
		bool mCanMoveToPlayer = false;
	
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Move Speed"))
		float mSpeed = 0;
	
	UPROPERTY()
		float mCurrentSpeed = 0;
	
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Acceleration"))
		float mAcceleration = 0;

	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "Use Debug Acquisition Range"))
		bool mUseDebugAcquisition = false;

	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "Debug Acquisition Range Color", EditCondition = "mUseDebugAcquisition", EditConditionHides))
		FColor mColorDebugAcquisition = FColor::White;

public:
	FORCEINLINE float GetRangeAcquisition() const { return mRangeAdd; };
	FORCEINLINE void SetRangeAcquisition(const float& _range) { mRangeAdd = _range; };

	FORCEINLINE Action<>& OnRangeAcquisition() { return mOnRangeAcquisition; };

public: 
	AG_Soul();

protected:
	virtual void BeginDestroy() override;
	/// <summary>
	/// Init Possess Player
	/// </summary>
	virtual void InitPickable() override;
	/// <summary>
	/// Init Events
	/// </summary>
	virtual void InitEvents() override;
	/// <summary>
	/// Reset Events
	/// </summary>
	virtual void ResetEvents() override;
	/// <summary>
	/// Init Timer Dropping
	/// </summary>
	void InitTimerDropping();
	/// <summary>
	/// Soul Picked Up VFX/SFX (implemented in BP)
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "On Soul Picked Up")
	void OnSoulPickedUp();
	/// <summary>
	/// Soul Move Begin VFX/SFX (implemented in BP)
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "On Soul Move Begin")
	void OnSoulMoveBegin();
	/// <summary>
	/// Soul Move In Continue VFX/SFX (implemented in BP)
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "On Soul Move In Continue")
	void OnSoulMoveInContinue();

private:
	/// <summary>
	/// Move to mPlayer with mSpeed
	/// </summary>
	/// <param name="DeltaTime">: Tick DeltaTime to smooth</param>
	void MoveToPlayer(const float& DeltaTime);
	/// <summary>
	/// Check Interaction with player
	/// </summary>
	virtual void CheckInteraction() override;
	/// <summary>
	/// Check Range to Add Soul
	/// </summary>
	void CheckRangeAcquisition();
	/// <summary>
	/// Add Soul To Player Inventory
	/// </summary>
	void AddToPlayerInventory();
	/// <summary>
	/// Draw Debugs
	/// </summary>
	virtual void DrawDebug() override;
	
};
