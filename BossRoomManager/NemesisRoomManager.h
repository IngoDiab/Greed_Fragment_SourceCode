#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h" 
#include "Greed/Utils/Action/Action.h"
#include "NemesisRoomManager.generated.h"

class AG_Nemesis;
class AG_Mob;
class AG_MainCharacter;
class UG_SpawnerMobile;

UCLASS()
class GREED_API ANemesisRoomManager : public AActor
{
	GENERATED_BODY()

	Action<> mOnBeginBoss = Action<>();
	Action<> mOnEndBoss = Action<>();	

	UPROPERTY(EditAnywhere, Category = "Trigger", meta = (DisplayName = "Trigger Boss Fight"))
	UBoxComponent* mTrigger = nullptr;

	UPROPERTY(EditAnywhere, Category = "Boss", meta = (DisplayName = "Nemesis Boss"))
	TSoftObjectPtr<AG_Nemesis> mNemesis = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Player", meta = (DisplayName = "Player"))
	AG_MainCharacter* mPlayer = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Player", meta = (DisplayName = "Player Mobile Spawner"))
	UG_SpawnerMobile* mPlayerMobileSpawner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Arena", meta = (DisplayName = "NavModifier to delete on Boss' death"))
	TArray<AActor*> mNavModifiers = TArray<AActor*>();

	UPROPERTY(VisibleAnywhere, Category = "Settings", meta = (DisplayName = "Mobs (besides Boss)"))
	TArray<AG_Mob*> mMobs = TArray<AG_Mob*>();

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Limit mob (besides Boss)"))
	uint8 mLimitMob = 4;
	
public:	
	ANemesisRoomManager();

private:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void GetPlayerRef();
	UFUNCTION()
	void AddToMobs(AG_Mob* _mob);
	UFUNCTION()
	void RemoveFromMobs(AG_Mob* _mob);
	bool IsUnderLimitMobs();
	void KillAll();
	void ActiveBoss(const bool& _activate);
	UFUNCTION()
	void EnterTrigger(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	void CleanArena();
};
