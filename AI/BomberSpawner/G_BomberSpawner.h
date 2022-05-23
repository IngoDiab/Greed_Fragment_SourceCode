#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/G_Mob.h"
#include "Greed/AI/DataTable/BoomerSpawner/DataBoomerSpawner.h"
#include "Greed/Utils/Struct/VisualDebug/G_VisualDebug.h"
#include "G_BomberSpawner.generated.h"

class AG_Boomer;

UCLASS()
class GREED_API AG_BomberSpawner : public AG_Mob
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "BomberSpawner | Components", EditAnywhere, meta = (DisplayName = "Mesh"), BlueprintReadWrite)
		UStaticMeshComponent* mMesh = nullptr;
	
	UPROPERTY()
		FTimerHandle mTimerHandle = FTimerHandle();
	
	UPROPERTY(Category = "BomberSpawner | Settings", EditAnywhere, meta = (DisplayName = "Spawn Boomer FX Name"))
		FString mSpawnBoomerFXName = "SpawnBoomer";
		
	UPROPERTY(Category="BomberSpawner | Settings", EditAnywhere, meta = (DisplayName = "Delay for spawn"))
		float mDelayForSpawn = 5.0f;

	UPROPERTY(Category="BomberSpawner | Settings", EditAnywhere, meta = (DisplayName = "Radius spawner"))
		float mRadiusSpawner = 300;

	UPROPERTY(Category="BomberSpawner | Settings", EditAnywhere, meta = (DisplayName = "current nb boomer"))
		int mCurrentNbBoomer = 0;

	UPROPERTY(Category="BomberSpawner | Settings", EditAnywhere, meta = (DisplayName = "max nb boomer"))
		int mMaxNbBoomer = 3;

	UPROPERTY(Category="BomberSpawner | Settings", EditAnywhere, meta = (DisplayName = "Bomber Prefab"))
		TSubclassOf<AG_Boomer> mBoomerPrefab = nullptr;

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return mMesh; }

public:	
	AG_BomberSpawner();
	virtual void BeginDestroy() override;
	virtual void OnDrawGizmos() override;

protected:
	virtual void InitMob() override;
	virtual void InitStatsFromDatatable(FG_EnemyData& _data) override;

private:
	void SpawnerDeath();
	UFUNCTION() void SpawnMob();

public:
	void RemoveBoomer();
};