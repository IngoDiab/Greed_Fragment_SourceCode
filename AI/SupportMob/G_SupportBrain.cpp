#include "G_SupportBrain.h"
#include "Greed/FSM/FSMComponent/G_FSMComponent.h"
#include "Greed/AI/Component/G_MovementComponent.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"

#include "Greed/FSM/FSMState/IdleState/G_IdleState.h"
#include "Greed/AI/Artificier/FleeState/G_FleeState.h"
#include "Greed/FSM/FSMState/AfterHitState/G_AfterHitState.h"
#include "Greed/FSM/FSMState/RelocateState/G_RelocateState.h"
#include "Greed/AI/DataTable/Support/DataSupport.h"

void UG_SupportBrain::InitFSM()
{
	//Create States
	UG_IdleState* _idleState = NewObject<UG_IdleState>();
	UG_FleeState* _fleeState = NewObject<UG_FleeState>();
	UG_AfterHitState* _afterHitState = NewObject<UG_AfterHitState>();
	UG_RelocateState* _relocateState = NewObject<UG_RelocateState>();

	//Transition from IDLE
	FsmCreatorHelper(*_idleState, &mRelocateSteering, _relocateState);
	FsmCreatorHelper(*_idleState, &mNeedToFlee, _fleeState);
	FsmCreatorHelper(*_idleState, &mRepositionAfterHit, _afterHitState);

	//Transition from FLEE
	FsmCreatorHelper(*_fleeState, &mNeedToFlee, _idleState, true);

	//Transition from RELOCATION AFTER GETTING HIT
	FsmCreatorHelper(*_afterHitState, &mRepositionAfterHit, _idleState, true);

	//Transition from RELOCATION DUE TO STEERING
	FsmCreatorHelper(*_relocateState, &mRelocateSteering, _idleState, true);

	//Initialize States
	if (_idleState) _idleState->InitState(this);
	if (_fleeState) _fleeState->InitState(this);
	if (_afterHitState) _afterHitState->InitState(this);
	if (_relocateState) _relocateState->InitState(this);

	if (mFSM) mFSM->StartFSM(_idleState);
}

void UG_SupportBrain::Update(const float _deltaTime)
{
	Super::Update(_deltaTime);
	if (mFleeComponent) mFleeComponent->UpdateFleeSystem();
	if (mMovementSystem) mMovementSystem->LookAtPosition();
	if (mFightSystem && mOwner && !mOwner->IsDead()) mFightSystem->UseAutoRepeatableSkills();
}

void UG_SupportBrain::CreateListener()
{
	Super::CreateListener();
	CreateMovementListener();
	CreateSteeringListener();
	CreateFleeListener();
	if (mOwner)mOwner->OnMobHitPureDamage() += [this](float _damage)
	{
		mRepositionAfterHit = true;
	};
}

void UG_SupportBrain::CreateMovementListener()
{
	if (!mMovementSystem) return;
	mMovementSystem->OnPositionReached() += [this]()
	{
		mNeedToFlee = false;
		mRepositionAfterHit = false;
		mMovementSystem->StopMove();
		mPawnMovementComponent->Velocity = FVector::ZeroVector;
	};
}

void UG_SupportBrain::CreateSteeringListener()
{
	if (!mSteeringComponent) return;
	mSteeringComponent->OnMobNear() += [this]()
	{
		if (!mCanRelocateSteering) return;
		mRelocateSteering = true;
	};

	mSteeringComponent->OnNoMobNear() += [this]()
	{
		if (!mCanRelocateSteering) return;
		mRelocateSteering = false;
	};
}

void UG_SupportBrain::CreateFleeListener()
{
	if (!mFleeComponent) return;
	mFleeComponent->OnTargetTooClose() += [this]()
	{
		mNeedToFlee = true;
	};
}

void UG_SupportBrain::CreateFightListener()
{
	Super::CreateFightListener();
	if (!mFightSystem) return;
	mFightSystem->OnAttack() += [this](const uint8& _slotSkill)
	{
		mFightSystem->UseSkill(_slotSkill);
	};
}

void UG_SupportBrain::InitData(FDataMob& _data)
{
	Super::InitData(_data);
	if (!mMovementSystem) return;
	mMovementSystem->SetRotationSpeed(_data.GetRotationSpeed());
	if (!mFleeComponent) return;
	FDataSupport* _dataSupport = StaticCast<FDataSupport*>(&_data);
	if (!_dataSupport) return;
	mFleeComponent->SetFleeDatas(_dataSupport->GetFleeDatas());
	mMovementSystem->SetDistanceToDodge(_dataSupport->GetDistanceToDodge());
	mMovementSystem->SetAngleToDodge(_dataSupport->GetAngleToDodge());
}

bool UG_SupportBrain::IsValid() const
{
	return Super::IsValid() && mMovementSystem && mSteeringComponent && mPawnMovementComponent && mFleeComponent;
}

#pragma region Override Brain
void UG_SupportBrain::Stun()
{
	Super::Stun();
	if (!mMovementSystem) return;
	mMovementSystem->StopMove();
}
#pragma endregion