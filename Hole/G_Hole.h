#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Greed/AI/Enum/G_ETypeMob.h"
#include "G_Hole.generated.h"

class UBoxComponent;
class UG_RepulsionComponent;
class UG_SteeringComponent;

UCLASS()
class GREED_API AG_Hole : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadWrite, meta = (DisplayName = "Static Mesh"))
	UStaticMeshComponent* mMesh = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (DisplayName = "Box Collider"))
	UBoxComponent* mBoxCollider = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (DisplayName = "End Box Collider"))
	UBoxComponent* mEndBoxCollider = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (DisplayName = "Activate Repulsion"))
	bool mRepulsionActive = false;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (DisplayName = "Repulsion Component"))
	UG_RepulsionComponent* mRepulsionComponent = nullptr;

#pragma region Repulsor
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Mobs Affected"))
	TArray<G_ETypeMob> mTypesMobsAffected = TArray<G_ETypeMob>();
#pragma endregion
	
public:	
	AG_Hole();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void EnterTrigger(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION()
	void ExitTrigger(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);
	
private:
	void AddListener();
	UG_SteeringComponent* GetSteering(AActor& _actor) const;
};
