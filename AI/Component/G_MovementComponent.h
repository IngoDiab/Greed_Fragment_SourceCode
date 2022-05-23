#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Greed/AI/G_BaseAiComponent.h"
#include "Greed/Utils/Action/Action.h"
#include "G_MovementComponent.generated.h"

class UNavigationSystemV1;
class AG_MainCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GREED_API UG_MovementComponent : public UG_BaseAiComponent
{
	GENERATED_BODY()

private:
	Action<> mOnMove = Action<>();
	Action<> mOnPositionReached = Action<>();

protected:
	UPROPERTY(Category="Movement Component", VisibleAnywhere, meta = (DisplayName = "Ai Controller"))
	AAIController* mAiController = nullptr;

	UPROPERTY()
	UNavigationSystemV1* mNavigationSystem = nullptr;

	UPROPERTY()
	UWorld* mWorld = nullptr;
	
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (DisplayName = "Is Focus On Player"))
	bool mFocusOnPlayer = false;
	
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (DisplayName = "Use Custom Acceptance Radius"))
	bool mUseCustomRadius = false;
	
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (DisplayName = "Acceptance Radius", EditCondition = "mUseCustomRadius", EditConditionHides))
	float mAcceptanceRadius = 250;
	
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (DisplayName = "Speed Rotate"))
	float mSpeedRotate = 1000;
	
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (DisplayName = "Distance LookAt"))
	float mOffsetDistanceLookAt = 1000;
	
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (DisplayName = "Can Move"))
	bool mCanMove = true;

	UPROPERTY(Category = "Movement", EditAnywhere, meta = (DisplayName = "Can Rotate"))
	bool mCanRotate = true;
	
	UPROPERTY(Category = "Movement", EditAnywhere, meta = (DisplayName = "Draw Debug"))
	bool mDrawDebug = false;

	UPROPERTY(Category="Movement Component", VisibleAnywhere, meta = (DisplayName = "Pos To Go"))
	FVector mPosToGo = FVector::ZeroVector;
	
	UPROPERTY(Category="Movement Component", VisibleAnywhere, meta = (DisplayName = "Pos To Look"))
	FVector mPosToLook = FVector::ZeroVector;
	
	UPROPERTY(Category="Movement Component", VisibleAnywhere, meta = (DisplayName = "Next Path Point"))
	FVector mNextPathPoint = FVector::ZeroVector;

	UPROPERTY(Category="Movement Component", VisibleAnywhere, meta = (DisplayName = "Filter actif"))
	TSubclassOf<UNavigationQueryFilter> mNavmeshFilter = nullptr;
	
	UPROPERTY(Category="Movement Component | Dodge", VisibleAnywhere, meta = (DisplayName = "Angle To Dodge Next Shot"))
	float mAngleToDodge = 60;
	
	UPROPERTY(Category="Movement Component | Dodge", VisibleAnywhere, meta = (DisplayName = "Distance To Dodge Next Shot"))
	float mDistanceToDodge = 400;

public:
	FORCEINLINE Action<>& OnMove() { return mOnMove; };
	FORCEINLINE Action<>& OnPositionReached() { return mOnPositionReached; };
	
	FORCEINLINE bool IsLockRotateOnPlayer() const { return mFocusOnPlayer; };
	FORCEINLINE void LockRotateOnPlayer(const bool& _lockOnPlayer) { mFocusOnPlayer = _lockOnPlayer; };

	FORCEINLINE bool CanMove() const { return mCanMove; };
	FORCEINLINE void SetCanMove(const bool& _canMove) { mCanMove = _canMove; };

	FORCEINLINE bool CanRotate() const { return mCanRotate; };
	FORCEINLINE void SetCanRotate(const bool& _canRotate) { mCanRotate = _canRotate; };
	FORCEINLINE void SetRotationSpeed(const float& _rotationSpeed) { mSpeedRotate = _rotationSpeed; };
	FORCEINLINE void SetFilter(TSubclassOf<UNavigationQueryFilter>& _filter) { mNavmeshFilter = _filter; };

	FORCEINLINE FVector GetNextPathPoint() const { return mNextPathPoint; };

	FORCEINLINE FVector GetPosToGo() const { return mPosToGo; };
	FORCEINLINE void SetPosToGo(const FVector& _posToGo) { mPosToGo = _posToGo; };
	
	FORCEINLINE FVector GetPosToLook() const { return mPosToLook; };
	FORCEINLINE void SetPosToLook(const FVector& _posToLook) { mPosToLook = _posToLook; };
	
	FORCEINLINE float GetAngleToDodge() const { return mAngleToDodge; };
	FORCEINLINE void SetAngleToDodge(const float& _angleToDodge) { mAngleToDodge = _angleToDodge; };
	FORCEINLINE float GetDistanceToDodge() const { return mDistanceToDodge; };
	FORCEINLINE void SetDistanceToDodge(const float& _distanceToDodge) { mDistanceToDodge = _distanceToDodge; };

public:
	UG_MovementComponent();

private:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	void RefreshNextPathPoint();
	void UpdateMovementSystem();
	void MoveTo();
	void StopMove();
	bool IsAtPos(const FVector& _posToGo, const float& _minDist) const;
	void LookAtPosition() const;
	FVector GetRandomPointInRadius(const FVector _center, const float _radius) const;

	virtual bool IsValid() const override;
};
