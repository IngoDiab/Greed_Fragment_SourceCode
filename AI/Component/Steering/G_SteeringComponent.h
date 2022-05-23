#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/G_BaseAiComponent.h"
#include "Greed/Utils/Action/Action.h"
#include "G_SteeringComponent.generated.h"

class UG_AIManager;
class AG_Mob;
class UG_RepulsionComponent;

UCLASS()
class GREED_API UG_SteeringComponent : public UG_BaseAiComponent
{
	GENERATED_BODY()

	Action<AG_Mob*> mOnAddMobNearby = Action<AG_Mob*>();
	Action<AG_Mob*> mOnRemoveMobNearby = Action<AG_Mob*>();

	Action<> mOnMobNear = Action<>();
	Action<> mOnNoMobNear = Action<>();
	Action<FVector> mOnPathUdpatedSteering = Action<FVector>();

	UPROPERTY(EditAnywhere, Category = "References", meta = (DisplayName = "Spawner Manager"))
	UG_AIManager* mAIManager = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Avoidement Multiplicator"))
	float mAvoidementMultiplicator = 10;
	
	//UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Avoidement Multiplicator Check"))
	//float mAvoidementMultiplicatorCheck = 10;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Refresh Rate Steering"))
	float mRefreshRateSteering = .5f;
	
	UPROPERTY(EditAnywhere, Category = "Debugs", meta = (DisplayName = "Use Debugs"))
	bool mUseDebugs = false;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (DisplayName = "Filter Steering"))
	TSubclassOf<UNavigationQueryFilter> mNavmeshFilter = nullptr;

	UPROPERTY(EditAnywhere, Category = "Debugs", meta = (DisplayName = "Avoidement Force"))
	FVector mAvoidementForce = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Category = "Debugs", meta = (DisplayName = "Buffered Avoidement Force"))
	FVector mBufferedAvoidementForce = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Category = "Debugs", meta = (DisplayName = "Near mob"))
	TArray<AG_Mob*> mNearMobs = TArray<AG_Mob*>();

	UPROPERTY(EditAnywhere, Category = "Debugs", meta = (DisplayName = "Repulsive Actors Component"))
	TArray<UG_RepulsionComponent*> mRepulsiveActorsComponent = TArray<UG_RepulsionComponent*>();
	
	UPROPERTY()
	UWorld* mWorld = nullptr;
	
	UPROPERTY()
	FTimerHandle mRefreshSteering;

public:
	FORCEINLINE Action<>& OnMobNear() { return mOnMobNear; };
	FORCEINLINE Action<>& OnNoMobNear() { return mOnNoMobNear; };
	FORCEINLINE Action<FVector>& OnPathUdpatedSteering() { return mOnPathUdpatedSteering; };

public:
	FORCEINLINE TArray<AG_Mob*>& GetNearMobsArray() { return mNearMobs; };
	FORCEINLINE TArray<UG_RepulsionComponent*>& GetRepulsiveActorsComponent() { return mRepulsiveActorsComponent; };
	FORCEINLINE TSubclassOf<UNavigationQueryFilter> GetAvoidementFilter() const { return mNavmeshFilter; };
	FORCEINLINE FVector GetAvoidementForce() const { return mAvoidementForce; };
	FORCEINLINE float GetAvoidementMultiplicator() const { return mAvoidementMultiplicator; };
	//FORCEINLINE float GetAvoidementMultiplicatorCheck() const { return mAvoidementMultiplicatorCheck; };
	FORCEINLINE void ResetAvoidementForce() { mAvoidementForce = FVector::ZeroVector; };
	FORCEINLINE FVector GetAvoidementPos() const { return mOwner ? mOwner->GetActorLocation() - mOwner->GetMobHeight()*FVector::UpVector + mAvoidementForce * mAvoidementMultiplicator : FVector::ZeroVector; };
	
protected:
	UG_SteeringComponent();
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void Init();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DrawDebugs();

public:
	template <typename TypeToAdd>
	void Add(TypeToAdd& _elementToAdd, TArray<TypeToAdd*>& _arrayToAddIn);
	template <typename TypeToRemove>
	void Remove(TypeToRemove& _elementToRemove, TArray<TypeToRemove*>& _arrayToRemoveFrom);
	void RefreshSteering();

private:
	void GetNearMobs();
	UFUNCTION()
	void RemoveMobFromNearbyOnDie(AG_Mob* _mob);
	void ResetSteering();
	void CalculateMobsRepulsions(FVector& _resultForce);
	void CalculateActorsComponentRepulsions(FVector& _resultForce);
	void RefreshForce(const FVector& _currentAvoidingForce);
	FVector ClampSeparationForce(const FVector& _baseVector) const;
	template <typename TypeToCheck>
	bool Exist(const TypeToCheck& _elementToCheck, const TArray<TypeToCheck*>& _arrayToCheckIn) const;
	bool IsMobInSteeringOnes(const AG_Mob& _mob) const;
};

template <typename TypeToAdd>
void UG_SteeringComponent::Add(TypeToAdd& _elementToAdd, TArray<TypeToAdd*>& _arrayToAddIn)
{
	if (Exist(_elementToAdd, _arrayToAddIn)) return;
	_arrayToAddIn.Add(&_elementToAdd);
}

template <typename TypeToRemove>
void UG_SteeringComponent::Remove(TypeToRemove& _elementToRemove, TArray<TypeToRemove*>& _arrayToRemoveFrom)
{
	if (_arrayToRemoveFrom.Num() == 0 || !Exist(_elementToRemove, _arrayToRemoveFrom)) return;
	_arrayToRemoveFrom.Remove(&_elementToRemove);
}

template <typename TypeToCheck>
bool UG_SteeringComponent::Exist(const TypeToCheck& _elementToCheck, const TArray<TypeToCheck*>& _arrayToCheckIn) const
{
	return _arrayToCheckIn.Contains(&_elementToCheck);
}