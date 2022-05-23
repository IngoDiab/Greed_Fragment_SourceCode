#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/G_BaseMob.h"
#include "G_BaseMobSKM.generated.h"

UCLASS()
class GREED_API AG_BaseMobSKM : public AG_BaseMob
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh Mob", BlueprintReadWrite)
		USkeletalMeshComponent* mEntityMesh = nullptr;
	
public:
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const { return mEntityMesh; }

public:
	AG_BaseMobSKM();

protected:
	virtual void Tick(float DeltaTime) override;
	void RootMotion();
};
