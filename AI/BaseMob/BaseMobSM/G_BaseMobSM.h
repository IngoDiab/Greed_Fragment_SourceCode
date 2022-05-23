#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/G_BaseMob.h"
#include "G_BaseMobSM.generated.h"

UCLASS()
class GREED_API AG_BaseMobSM : public AG_BaseMob
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh Mob", BlueprintReadWrite)
	UStaticMeshComponent* mEntityMesh = nullptr;

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return mEntityMesh; }

public:
	AG_BaseMobSM();
	
};
