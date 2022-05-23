#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Greed/AI/DataTable/Mob/DataMob.h"
#include "Greed/Utils/Interface/Valid/Valid.h"
#include "Greed/Utils/Interface/VisualDebug/VisualDebug.h"
#include "Greed/Utils/Struct/VisualDebug/G_VisualDebug.h"

#include "G_Brain.generated.h"

class AG_BaseMob;
class UG_FSMComponent;
class UG_MovementComponent;
class UG_FightSystemComponent;
class UG_FSMState;
class UG_SkillComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable,BlueprintType)
class GREED_API UG_Brain : public UActorComponent, public IValid, public IVisualDebug
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AG_BaseMob* mOwner = nullptr;

	UPROPERTY(Category="Brain | Debug", EditAnywhere, meta = (DisplayName = "Visual Debug"))
	FG_VisualDebug mVisualDebug = FG_VisualDebug();
	
	UPROPERTY(Category = "Brain | FSM", VisibleAnywhere)
	UG_FSMComponent* mFSM = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Fight Component"))
	UG_FightSystemComponent* mFightSystem = nullptr;

	UPROPERTY(Category = "Brain | AiComponent", EditAnywhere, meta = (DisplayName = "Skill Component"))
	UG_SkillComponent* mSkillComponent = nullptr;

	UPROPERTY(Category = "Brain | FSM Bool", VisibleAnywhere, BlueprintReadOnly)
	bool mIsStun = false;
	
public:
	FORCEINLINE void SetFSMComponent(UG_FSMComponent& _FSMComp) { mFSM = &_FSMComp; }
	FORCEINLINE UG_FSMComponent* GetFSMComponent() const { return mFSM; }

	FORCEINLINE void SetFightComponent(UG_FightSystemComponent& _fightComp) { mFightSystem = &_fightComp; }
	FORCEINLINE UG_FightSystemComponent* GetFightComponent() const { return mFightSystem; }

	FORCEINLINE void SetSkillComponent(UG_SkillComponent& _skillComp) { mSkillComponent = &_skillComp; }
	FORCEINLINE UG_SkillComponent* GetSkillComponent() const { return mSkillComponent; }
	FORCEINLINE AG_BaseMob* GetMobOwner() const { return mOwner; }

public:
	UG_Brain();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void Init();

public:
	virtual void InitData(FDataMob& _data);
	virtual void InitFSM();

protected:
	virtual void InitOwner();

	virtual void Update(const float _deltaTime);
	void UpdateFightSystem();

	virtual void CreateListener();
	virtual void CreateFightListener();

	void FsmCreatorHelper(UG_FSMState& _currentState, bool* _condition, UG_FSMState* _nextState, bool _reverse = false) const;

public:
	virtual bool IsValid() const override;
	virtual void Stun();
	void EndStun();

#pragma region Override
#pragma region IVisualDebug
public:
	virtual bool ShowDebug() const override;
	virtual float DebugSize() const override;
	virtual float DebugHeight() const override;
	virtual FColor DebugValidColor() const override;
	virtual FColor DebugErrorColor() const override;
	virtual void OnDrawGizmos() override;
	void StartFsm() const;
#pragma endregion IVisualDebug
#pragma endregion Override
};
