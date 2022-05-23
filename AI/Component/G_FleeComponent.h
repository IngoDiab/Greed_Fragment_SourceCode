#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Greed/AI/G_BaseAiComponent.h"
#include "Greed/Weapons/Weapon/G_Weapon.h"
#include "Greed/AI/Artificier/FleeState/FleeStruct/G_FleeStruct.h"
#include "Greed/Utils/Action/Action.h"
#include "G_FleeComponent.generated.h"

class UNavigationSystemV1;
class AG_MainCharacter;

UCLASS()
class GREED_API UG_FleeComponent : public UG_BaseAiComponent
{
	GENERATED_BODY()
	
private:
	Action<> mOnTargetTooClose = Action<>();
	Action<> mOnTargetFarEnough = Action<>();

private:
	UPROPERTY()
		UNavigationSystemV1* mNavigationSystem = nullptr;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Filter actif"))
		TSubclassOf<UNavigationQueryFilter> mNavmeshFilterBP = nullptr;;

	UPROPERTY(VisibleAnywhere)
		TScriptInterface<ITarget> mTarget = nullptr;

	UPROPERTY()
		FVector mPosToFlee = FVector::ZeroVector;
	
	UPROPERTY()
		FVector mCurrentFleeDirection = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
		TMap<ETypeWeapon, FG_FleeStruct> mFleeDatas = TMap<ETypeWeapon, FG_FleeStruct>();
	
	UPROPERTY(EditAnywhere)
		ETypeWeapon mCurrentTargetWeapon = ETypeWeapon::NONE;
	
	UPROPERTY(EditAnywhere)
		FG_FleeStruct mCurrentFleeStruct = FG_FleeStruct();

	UPROPERTY(VisibleAnywhere)
		bool mIsFleeing = false;
	
	UPROPERTY(EditAnywhere)
		bool mShowDebug = true;

public:
	FORCEINLINE Action<>& OnTargetTooClose() { return mOnTargetTooClose; };
	FORCEINLINE Action<>& OnTargetFarEnough() { return mOnTargetFarEnough; };


public:
	FORCEINLINE void SetFilter(TSubclassOf<UNavigationQueryFilter>& _filter) { mNavmeshFilterBP = _filter; };
	FORCEINLINE void SetPosToFlee(const FVector& _pos) { mPosToFlee = _pos; };
	FORCEINLINE FVector GetPosToFlee() const { return mPosToFlee; };
	FORCEINLINE void SetFleeDatas(const TMap<ETypeWeapon, FG_FleeStruct>& _fleeDatas) { mFleeDatas = _fleeDatas; };
	FORCEINLINE bool NeedToFlee() const { return mIsFleeing; };

private:
	virtual void BeginPlay() override;
	virtual void InitOwner() override;

	void InitTarget();
	void RefreshFleeStruct(const ITarget& _target);
	void ChooseFleeStruct(const ETypeWeapon& _typeWeapon);
	virtual void BeginDestroy() override;
	bool IsPointOnNavMesh(FVector _pos) const;

public:
	void UpdateFleeSystem();
	FVector FindFleeingPoint() const;
	FVector FindBestDirection(const FVector& _targetPosToMobPos, const FVector& _targetPos) const;
	FVector RotateDirection(const FVector& _targetPosToMobPos, const int _angle) const;
	virtual void DrawDebugs();
};
