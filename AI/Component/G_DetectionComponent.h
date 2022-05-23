#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/G_BaseAiComponent.h"
#include "Greed/Utils/Action/Action.h"
#include "G_DetectionComponent.generated.h"

class AG_BaseMob;
class AG_MainCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GREED_API UG_DetectionComponent : public UG_BaseAiComponent
{
	GENERATED_BODY()

private:
	DECLARE_EVENT(UG_DetectionComponent, TargetDetected)
	TargetDetected mOnTargetDetected;
	
	Action<> mOnTargetLoose = Action<>();

private:
	UPROPERTY(VisibleAnywhere)
	TScriptInterface<ITarget> mTarget = nullptr;
	
	UPROPERTY(Category="Detection | Settings", VisibleAnywhere, meta = (DisplayName = "Detection Range"))
	float mDetectionRange = 900;
	
	UPROPERTY(Category="Detection | Debug", VisibleAnywhere, meta = (DisplayName = "Target detected ?"))
	bool mTargetDetected = false;

public:
	FORCEINLINE TargetDetected& OnTargetDetected() { return mOnTargetDetected; };
	FORCEINLINE Action<>& OnTargetLoose() { return mOnTargetLoose; };

public:
	FORCEINLINE void SetDetectionRange(const float& _detectionRange) { mDetectionRange = _detectionRange; };
	FORCEINLINE float GetDetectionRange() const { return mDetectionRange; };

	FORCEINLINE void SetTargetDetected(const bool& _detected) { mTargetDetected = _detected; };
	FORCEINLINE bool IsTargetDetected() const { return mTargetDetected; };

public:
	UG_DetectionComponent();

private:
	virtual void BeginPlay() override;
	void InitTarget();
	virtual void BeginDestroy() override;

public:
	void UpdateDetection();
	bool IsInRange(const FVector& _target, const float& _dist)const;
};
