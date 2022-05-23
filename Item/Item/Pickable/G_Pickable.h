#pragma once

#include "CoreMinimal.h"
#include "../G_Item.h"
#include "Greed/Utils/Action/Action.h"
#include "G_Pickable.generated.h"

class UG_ItemHandler;
class AG_MainCharacter;

UCLASS(ABSTRACT)
class GREED_API AG_Pickable : public AG_Item
{
	GENERATED_BODY()

protected:
	Action<> mOnInitPickable = Action<>();
	Action<float> mOnUpdatePickable = Action<float>();

	Action<> mOnRangeInteraction = Action<>();
	Action<> mOnRangeInteractionLost = Action<>();

	//UPROPERTY()
		//UG_LevelItemManager* mLevelItemManager = nullptr;

	UPROPERTY()
		UG_ItemHandler* mItemHandler = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
		AG_MainCharacter* mPlayer = nullptr;
	
	UPROPERTY()
		float mDistance = TNumericLimits<float>::Max();
	
	UPROPERTY(EditAnywhere, Category = "Range", meta = (DisplayName = "Range Interaction"))
		float mRangeInteraction = 0;

	UPROPERTY(EditAnywhere, Category = "Drop", meta = (DisplayName = "Drop Speed"))
		float mDropSpeed = 0;
	
	UPROPERTY()
		FVector mDropPos = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Rotate Speed"))
		float mSpeedRotate = 50;

	UPROPERTY()
		FVector mCompUpOffset = FVector::ZeroVector;

	UPROPERTY()
		FVector mCompBasicLocation = FVector::ZeroVector;

	UPROPERTY()
		bool mMovingUp = true;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Up Down Speed"))
		float mUpDownSpeed = 15;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Up Offset"))
		float mUpOffset = 50;
	
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "Use Debug Interaction Range"))
		bool mUseDebugInteraction = false;

	UPROPERTY(EditAnywhere, Category = "Debug", meta = (DisplayName = "Debug Interaction Range Color", EditCondition = "mUseDebugInteraction", EditConditionHides))
		FColor mColorDebugInteraction = FColor::White;
	
	UPROPERTY()
		bool mInGame = false;

public:
	FORCEINLINE float GetRangeInteraction() const { return mRangeInteraction; };
	FORCEINLINE void SetRangeInteraction(const float& _range) { mRangeInteraction = _range; };

	FORCEINLINE void SetDropPosition(const FVector& _pos) { mDropPos = _pos; };

	FORCEINLINE Action<>& OnInitPickable() { return mOnInitPickable; };
	FORCEINLINE Action<float>& OnUpdatePickable() { return mOnUpdatePickable; };

	FORCEINLINE Action<>& OnRangeInteraction() { return mOnRangeInteraction; };
	FORCEINLINE Action<>& OnRangeInteractionLost() { return mOnRangeInteractionLost; };

protected:
	FORCEINLINE bool ShouldTickIfViewportsOnly() const { return true; };
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	AG_Pickable();
	virtual void PostInitProperties() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/// <summary>
	/// Init Events
	/// </summary>
	virtual void InitEvents();
	/// <summary>
	/// Reset Events
	/// </summary>
	virtual void ResetEvents();
	/// <summary>
	/// Init Possess Player
	/// </summary>
	virtual void InitPickable();
	/// <summary>
	/// Init Level Item Manager
	/// </summary>
	void InitItemHandler();
	/// <summary>
	/// Calculate Distance with Player
	/// </summary>
	void UpdateDistance();
	/// <summary>
	/// Check if is at range from player
	/// </summary>
	/// <param name="_range">: Range to check</param>
	/// <returns>true if at range, false otherwise</returns>
	bool IsAtRange(const float& _range) const;
	/// <summary>
	/// Drop Animation
	/// </summary>
	/// <param name="DeltaTime">: Tick DeltaTime to smooth</param>
	/// <param name="_targetPos">: Position where to drop</param>
	virtual void DropMovement(const float& DeltaTime, const FVector& _targetPos);
	void InitMovementScene(USceneComponent& _compToMove);
	/// <summary>
	/// Up Down Movement In Scene
	/// </summary>
	/// <param name="DeltaTime">: DeltaTime to smooth</param>
	virtual void UpDown(const float& DeltaTime, USceneComponent& _compToMove);
	/// <summary>
	/// Rotation Movement In Scene
	/// </summary>
	/// <param name="DeltaTime">: DeltaTime to smooth</param>
	virtual void Rotate(const float& DeltaTime, USceneComponent& _compToRotate);
	/// <summary>
	/// Check If Comp Is At Pos
	/// </summary>
	/// <param name="_pos">: Pos to check</param>
	/// <returns>true if component is at pos, false otherwise</returns>
	bool CompIsAtPos(const FVector& _pos, const USceneComponent& _comp) const;
	/// <summary>
	/// Check Interaction with player
	/// </summary>
	virtual void CheckInteraction();
	/// <summary>
	/// Draw Debugs
	/// </summary>
	virtual void DrawDebug();
};
