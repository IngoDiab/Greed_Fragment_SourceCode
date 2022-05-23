#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/BaseMobSM/G_BaseMobSM.h"
#include "Greed/Skills/Skill/G_Skill.h"
#include "Greed/AI/DataTable/Nemesis/DataNemesis.h"
#include "Greed/AI/Nemesis/Phase/G_Phase.h"
#include "G_Nemesis.generated.h"

class AG_NemesisDestruct;

UCLASS()
class GREED_API AG_Nemesis : public AG_BaseMobSM
{
	GENERATED_BODY()

protected:
	Action<> mOnActivateBoss = Action<>();
	Action<uint8> mOnChangePhase = Action<uint8>();
	Action<> mOnEndDestruction = Action<>();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Activate", meta = (DisplayName = "Auto Activate"))
	bool mAutoActivate = false;

	UPROPERTY(VisibleAnywhere, Category = "Phases", meta = (DisplayName = "Current Phase Num"))
	uint8 mCurrentPhaseNum = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Phases", meta = (DisplayName = "Current Phase"))
	FG_Phase mCurrentPhase;

	UPROPERTY(VisibleAnywhere, Category = "Settings", meta = (DisplayName = "All Phases"))
	TArray<FG_Phase> mAllPhases = TArray<FG_Phase>();

	UPROPERTY(EditAnywhere, Category = "Rotate Mesh", meta = (DisplayName = "Actor to look"))
	TScriptInterface<ITarget> mTargetToLook = nullptr;

	UPROPERTY(EditAnywhere, Category = "Rotate Mesh", meta = (DisplayName = "All Pivot Neck"))
	TArray<USceneComponent*> mNecksToRotate = TArray<USceneComponent*>();

	UPROPERTY(EditAnywhere, Category = "Rotate Mesh", meta = (DisplayName = "All Pivot Neck"))
	TArray<FRotator> mNecksBasicsRotations = TArray<FRotator>();
	
	UPROPERTY(EditAnywhere, Category = "Rotate Mesh", meta = (DisplayName = "All Pivot Neck"))
	TArray<FVector> mNecksBasicsForwards = TArray<FVector>();
	
	UPROPERTY(EditAnywhere, Category = "Destruction", meta = (DisplayName = "Destructible Nemesis"))
	TSubclassOf<AG_NemesisDestruct> mDestructibleNemesis = nullptr;

	UPROPERTY()
	FTimerHandle mTransitionPhaseHandle;
	
	UPROPERTY()
	float mRotationSpeed = 10;
	
	UPROPERTY()
	float mMaxAngleTargetBasicForward = 45;

public:
	FORCEINLINE Action<uint8>& OnChangePhase() { return mOnChangePhase; };
	FORCEINLINE Action<>& OnEndDestruction() { return mOnEndDestruction; };
	FORCEINLINE uint8 GetCurrentPhase() { return mCurrentPhaseNum; }

protected:
	AG_Nemesis();
	void CreatePivotsNecks(const FName& _namePivot);
	virtual void InitMob() override;
	virtual void InitUI();
	virtual void InitStatsFromDatatable(FG_EnemyData& _data) override;
	UFUNCTION()
	void InitHPDueToPlayerWeapon(const FDataNemesis& _dataNemesis);
	void InitChangeTargetDelegate();
	void GetPlayerRef();
	virtual void ClearAction() override;
	virtual void Tick(float DeltaTime) override;
	void LookAtPlayerPos(const uint8& _index);
	void SetNextPhase();
	void EndSkills();

	virtual void CallSpecAchievements(class UG_AchievementManager* _achievementManager) override;

public:
	void Activate();
	virtual void IncreasePhase();
	FG_Phase GetPhase(const uint8& _index);
	virtual void TargetTakeDamage(const float& Life, ETypeDamage _typeDamage) override;

private:
	void SpawnDestructibleNemesis();
};
