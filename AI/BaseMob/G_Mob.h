#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Greed/3C/MainCharacter/Interface/Target/Target.h"
#include "Greed/3C/MainCharacter/Interface/TargetStats/TargetStats.h"
#include "Greed/3C/MainCharacter/Stats/StatsTarget/StatsTarget.h"
#include "Greed/AI/DataTable/G_EnemyData.h"
#include "Greed/Utils/Action/Action.h"
#include "Greed/Utils/Interface/Valid/Valid.h"
#include "Greed/Utils/Interface/VisualDebug/VisualDebug.h"
#include "Greed/Utils/Struct/VisualDebug/G_VisualDebug.h"
#include "Greed/AI/Enum/G_ETypeMob.h"
#include "NavFilters/NavigationQueryFilter.h"

#include "G_Mob.generated.h"

class AAIController;
class UG_FXManager;
class UG_AIManager;
class UG_UI_ComponentEnemy;
class UG_DropComponent;
class UNiagaraComponent;
class AG_DamageActor;

UCLASS()
class GREED_API AG_Mob : public APawn, public ITarget, public ITargetStats, public IVisualDebug, public IValid
{
	GENERATED_BODY()

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreDie, AG_Mob*, _mob);
	FPreDie mOnPreDie;

	Action<float> mOnTakeDamage = Action<float>();
	Action<float> mOnMobHitPureDamage = Action<float>();
	Action<float> mOnHealing = Action<float>();
	Action<float> mOnReceiveArmor = Action<float>();

	Action<> mOnDie = Action<>();
	Action<> mOnStun = Action<>();
	Action<> mOnEndStun = Action<>();
	Action<> mOnDrawGizmosEvent = Action<>();

protected:
	UPROPERTY(Category = "Manager", VisibleAnywhere, meta = (DisplayName = "Manager Mob"))
	UG_AIManager* mMobManager = nullptr;
	
	UPROPERTY(Category = "Manager", VisibleAnywhere, meta = (DisplayName = "Manager FX"))
	UG_FXManager* mFXManager = nullptr;

	UPROPERTY(Category = "BaseMob | FX", EditAnywhere, meta = (DisplayName = "Spawn FX Name"))
	FString mNameFXSpawn = "Spawn";
	
	UPROPERTY(Category = "BaseMob | FX", EditAnywhere, meta = (DisplayName = "Death FX Name"))
	FString mNameFXDeath = "Death";
	
	UPROPERTY(Category = "BaseMob | Settings", EditAnywhere, meta = (DisplayName = "DataBase Mob"))
	UDataTable* mDataTableMob = nullptr;

	UPROPERTY(Category = "BaseMob | Settings", EditAnywhere, meta = (DisplayName = "Name DataBase"))
	FName mDataTableName = "Preset1";
	
	UPROPERTY(Category = "BaseMob | Settings", EditAnywhere, meta = (DisplayName = "Type"))
	G_ETypeMob mType = G_ETypeMob::OTHER;

	UPROPERTY(Category = "BaseMob | Settings", EditAnywhere, meta = (DisplayName = "Filter Spawn & Movement"))
	TSubclassOf<UNavigationQueryFilter> mNavmeshFilter = nullptr;
	
	UPROPERTY(Category = "BaseMob | Settings", EditAnywhere, meta = (DisplayName = "Destroy After Death"))
	bool mDestroyAfterDeath = true;

	UPROPERTY(Category = "BaseMob | Settings", EditAnywhere, meta = (DisplayName = "Delay Destroy After Death", EditCondition = "mDestroyAfterDeath", EditConditionHides))
	float mDelayDestroy = 1;
	
	UPROPERTY()
	FTimerHandle mDelayDestroyHandler;
	
	UPROPERTY()
	FTimerHandle mStunTimer;

	UPROPERTY()
	UWorld* mWorld = nullptr;

	UPROPERTY()
	AAIController* mAiController = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, meta = (DisplayName = "Capsule Component"))
	UCapsuleComponent* mCapsuleComponent = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, meta = (DisplayName = "Effect Component"))
	UG_EffectComponent* mEffectComponent = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, meta = (DisplayName = "UI Component"))
	UG_UI_ComponentEnemy* mUiComponent = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, meta = (DisplayName = "Drop Component"))
	UG_DropComponent* mDropComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "stats", meta = (DisplayName = "Stats Target"))
	FStatsTarget mStatsTarget = FStatsTarget();

	UPROPERTY(Category = "BaseMob | Settings", EditAnywhere, meta = (DisplayName = "Niagara System"))
	UNiagaraComponent* mNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Is Stun ?")
	bool mIsStun = false;

	UPROPERTY(Category = "BaseMob | Debug", VisibleAnywhere, meta = (DisplayName = "Activate God Mod"))
	bool mGodMod = false;

	UPROPERTY()
	bool mIsDead = false;

	UPROPERTY(Category = "Stats", EditAnywhere, meta = (DisplayName = "Healer Weight", AllowPrivateAccess = true), BlueprintReadOnly)
	int mHealerWeight = 1;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Radius Steering"))
	float mRadius = 10;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Radius End Steering"))
	float mEndRadius = 10;

	UPROPERTY(Category = "Base Mob", EditAnywhere, meta = (DisplayName = "Visual Debug"))
	FG_VisualDebug mVisualDebug = FG_VisualDebug();

	UPROPERTY(Category = "BaseMob | Utils", EditAnywhere)
	TSubclassOf<AG_DamageActor> mDamageActorClass = nullptr;

public:
	FORCEINLINE Action<float>& OnTakeDamage() { return mOnTakeDamage; };
	FORCEINLINE Action<float>& OnMobHitPureDamage() { return mOnMobHitPureDamage; };
	FORCEINLINE Action<float>& OnHealing() { return mOnHealing; };
	FORCEINLINE Action<float>& OnReceiveArmor() { return mOnReceiveArmor; };
	FORCEINLINE FPreDie& OnPreDie() { return mOnPreDie; };
	FORCEINLINE Action<>& OnDie() { return mOnDie; };
	FORCEINLINE Action<>& OnStun() { return mOnStun; };
	FORCEINLINE Action<>& OnEndStun() { return mOnEndStun; };
	FORCEINLINE Action<>& OnDrawGizmosEvent() { return mOnDrawGizmosEvent; };

public:
	FORCEINLINE G_ETypeMob GetType() const { return mType; };
	FORCEINLINE AAIController* GetAiController() { return mAiController; };
	FORCEINLINE UCapsuleComponent* GetCapsule() const { return mCapsuleComponent; }
	FORCEINLINE UNiagaraComponent* GetNiagaraComponent() const { return mNiagaraComponent; }
	FORCEINLINE TSubclassOf<UNavigationQueryFilter> NavMeshFilter()const { return mNavmeshFilter; }
	FORCEINLINE float GetMobHeight() const { return mCapsuleComponent ? mCapsuleComponent->GetScaledCapsuleHalfHeight() : 1; };
	FORCEINLINE float GetMobWidth() const { return mCapsuleComponent ? mCapsuleComponent->GetScaledCapsuleRadius() : 1; };
	FORCEINLINE void Kill(const ETypeDamage& _type, const bool& _forceKillEvenInGodMod) { TargetTakeDamage(Life(), _type, _forceKillEvenInGodMod); };
	FORCEINLINE bool IsDead() const { return this == nullptr ? true : mIsDead; };
	FORCEINLINE int GetHealerWeight() const { return mHealerWeight; }
	FORCEINLINE float GetRadius() const { return mRadius; };
	FORCEINLINE float GetEndRadius() const { return mEndRadius; };
	FORCEINLINE void SetStun(const bool& _stun) { mIsStun = _stun; };
	FORCEINLINE void SetGodMod(const bool& _godMod) { mGodMod = _godMod; }
	FORCEINLINE bool IsInGodMod() const { return mGodMod; }

public:
	AG_Mob();

protected:
	virtual void BeginPlay() override;
	virtual void ClearAction();

	void CreateDamageActor(float _damage, bool _godMode);

	UFUNCTION()
		virtual void CallSpecAchievements(class UG_AchievementManager* _achievementManager);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetGodMod(float _time);
	virtual void Stun(float _stunDuration = -1);
	UFUNCTION()
	virtual void EndStun();

protected:
	virtual void InitMob();
	template <typename TypeDataStruct>

	TypeDataStruct* GetDataFromTable();
	virtual void InitStatsFromDatatable(FG_EnemyData& _data);
	virtual void InitComponentsWithData(FG_EnemyData& _data);
	virtual void InitEvents();
	virtual void InitRefs();
	void AddToMobManager(const bool& _add);

	void BeginDestroyAfterDeath();
	UFUNCTION(BlueprintImplementableEvent, Category = "On Death Mob VFX")
	void OnDeathVFX();
	virtual void EndDestroyAfterDeath();

	virtual void FreezeMob(const bool& _freeze);
	virtual void DropSouls();

	virtual void SpawnAnimation();
	void SpawnFX(const FString& _keyFX) const;

	void ClearTimeManager(FTimerHandle& _handler);

	/// <summary>
	/// On Spawn Mob SFX (implemented in BP)
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "On Spawn Mob SFX")
		void OnSpawnSFX();

	/// <summary>
	/// On Spawn Mob VFX (implemented in BP)
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "On Spawn Mob VFX")
		void OnSpawnVFX();

	/// <summary>
	/// On Death Mob SFX (implemented in BP)
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent, Category = "On Death Mob SFX")
		void OnDeathSFX();

	UFUNCTION()
		void CallAchievements();

#pragma region Override
public:
	virtual bool IsValid() const override;

	virtual float Life() const override;
	virtual float MaxLife() const override;
	virtual float Speed() const override;
	virtual float MaxSpeed() const override;

	virtual float DamageResistance() const override;
	virtual float MaxDamageResistance() const override;
	virtual float Armor() const override;
	virtual float MaxArmor() const override;

public:
	virtual void AddArmor(const float& _armor) override;
	//life
	virtual void TargetTakeDamage(const float& _life, ETypeDamage _typeDamage) override;
	virtual void TargetTakeDamage(const float& Life, ETypeDamage _typeDamage, const bool& _forceTakeDamageEvenGodMod);
	virtual void AddLife(const float& _life) override;
	virtual void SetMaxLife(const float& _maxLife) override;
	//speed
	virtual void SlowSpeed(const float& _speed) override;
	virtual void AddSpeed(const float& _speed) override;
	virtual void SetMaxSpeed(const float& _maxSpeed) override;
	virtual UG_EffectComponent* GetEffectComponent() const override;

	virtual bool ShowDebug() const override;
	virtual float DebugSize() const override;
	virtual float DebugHeight() const override;
	virtual FColor DebugValidColor() const override;
	virtual FColor DebugErrorColor() const override;
	virtual FVector TargetPosition() const override;
	virtual void OnDrawGizmos() override;
#pragma endregion Override
};

template <typename TypeDataStruct>
TypeDataStruct* AG_Mob::GetDataFromTable()
{
	if (!mDataTableMob || mDataTableName == "") return nullptr;
	TypeDataStruct* _dataToFill = mDataTableMob->FindRow<TypeDataStruct>(mDataTableName, "");
	return _dataToFill;
}
