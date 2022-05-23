#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Greed/Utils/Interface/Valid/Valid.h"
#include "Greed/AI/BaseMob/G_BaseMob.h"
#include "G_BaseAiComponent.generated.h"

class ITarget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GREED_API UG_BaseAiComponent : public UActorComponent, public IValid
{
	GENERATED_BODY()

#pragma region f/p
protected:
	UPROPERTY(VisibleAnywhere)
	AG_BaseMob* mOwner = nullptr;

public:
	FORCEINLINE FVector GetOwnerLocation() const {return mOwner ? mOwner->GetActorLocation() : FVector::ZeroVector; };
	FORCEINLINE FRotator GetOwnerRotation() const { return mOwner ? mOwner->GetActorRotation() : FRotator::ZeroRotator; };

	FORCEINLINE virtual bool IsValid() const override { return mOwner != nullptr; };
#pragma endregion

#pragma region constructor
public:
	UG_BaseAiComponent();
#pragma endregion constructor

#pragma region unreal methods
protected:
	virtual void BeginPlay() override;
#pragma endregion unreal methods

#pragma region Protected Methods
	virtual void InitOwner();


#pragma endregion Protected Methods
};
