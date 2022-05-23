#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "G_CustomPathFollowingComponent.generated.h"

class UNavigationSystemV1;
class UG_SteeringComponent;
class AG_AIController;

UCLASS()
class GREED_API UG_CustomPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
	
	UPROPERTY()
	AG_AIController* mController = nullptr;

	UPROPERTY(Category = "AI Components", EditAnywhere, meta = (DisplayName = "Steering Component"))
	UG_SteeringComponent* mSteeringComponent = nullptr;
	
	UPROPERTY(Category = "Debugs", EditAnywhere, meta = (DisplayName = "Use Debug"))
	bool mUseDebug = false;
	
	UPROPERTY(Category = "Debugs", EditAnywhere, meta = (DisplayName = "Path Points"))
	TArray<FVector> mPath;
	
	UPROPERTY(Category = "Debugs", EditAnywhere, meta = (DisplayName = "Buffered Avoidance"))
	FVector mBufferedAvoidance = FVector::ZeroVector;
	
public:
	FORCEINLINE virtual void SetSteeringComponent(UG_SteeringComponent& _steeringComp) { mSteeringComponent = &_steeringComp; }

protected:
	void BeginPlay();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual FAIRequestID RequestMove(const FAIMoveRequest& RequestData, FNavPathSharedPtr InPath) override;
	FVector PathTargetWithSteering(TArray<FNavPathPoint>& _currentPath, const uint8& _nbPoints);
};
