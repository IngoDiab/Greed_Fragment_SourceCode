#include "G_ArtificierBrain.h"

#include "Greed/AI/Component/G_FleeComponent.h"
#include "Greed/AI/Component/G_MovementComponent.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"
#include "Greed/FSM/FSMComponent/G_FSMComponent.h"
#include "Greed/AI/Animations/G_AnimBaseMob.h"

#include "Greed/FSM/FSMState/AtkState/G_AtkState.h"
#include "Greed/FSM/FSMState/ChaseState/G_ChaseState.h"
#include "Greed/FSM/FSMState/IdleState/G_IdleState.h"
#include "Greed/FSM/FSMState/WaitCDState/G_WaitCDState.h"
#include "Greed/FSM/FSMState/AfterHitState/G_AfterHitState.h"
#include "Greed/FSM/FSMState/RelocateState/G_RelocateState.h"
#include "FleeState/G_FleeState.h"

void UG_ArtificierBrain::InitFSM()
{
	//Create States
	UG_IdleState* _idleState = NewObject<UG_IdleState>();
	UG_ChaseState* _chaseState = NewObject<UG_ChaseState>();
	UG_AtkState* _atkState = NewObject<UG_AtkState>();
	UG_WaitCDState* _waitCDState = NewObject<UG_WaitCDState>();
	UG_FleeState* _fleeState = NewObject<UG_FleeState>();
	UG_AfterHitState* _afterHitState = NewObject<UG_AfterHitState>();
	UG_RelocateState* _relocateState = NewObject<UG_RelocateState>();

	//Transition from IDLE
	FsmCreatorHelper(*_idleState, &mPlayerTooFar, _chaseState);
	FsmCreatorHelper(*_idleState, &mRelocateSteering, _relocateState);
	FsmCreatorHelper(*_idleState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_idleState, &mPlayerTooClose, _fleeState);
	FsmCreatorHelper(*_idleState, &mWaitCooldown, _waitCDState);
	FsmCreatorHelper(*_idleState, &mRepositionAfterHit, _afterHitState);

	//Transition from CHASE
	FsmCreatorHelper(*_chaseState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_chaseState, &mAtRangeAttack, _waitCDState);
	FsmCreatorHelper(*_chaseState, &mRepositionAfterHit, _afterHitState);

	//Transition from ATTACK
	FsmCreatorHelper(*_atkState, &mCDWithoutAnim, _waitCDState);
	FsmCreatorHelper(*_atkState, &mRelocateSteering, _relocateState);
	FsmCreatorHelper(*_atkState, &mPlayerTooFar, _chaseState);
	FsmCreatorHelper(*_atkState, &mPlayerTooClose, _fleeState);
	FsmCreatorHelper(*_atkState, &mRepositionAfterHit, _afterHitState);
	
	//Transition from WAIT COOLDOWN
	FsmCreatorHelper(*_waitCDState, &mRelocateSteering, _relocateState);
	FsmCreatorHelper(*_waitCDState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_waitCDState, &mPlayerTooFar, _chaseState);
	FsmCreatorHelper(*_waitCDState, &mPlayerTooClose, _fleeState);
	FsmCreatorHelper(*_waitCDState, &mRepositionAfterHit, _afterHitState);

	//Transition from FLEE
	FsmCreatorHelper(*_fleeState, &mNeedToFlee, _idleState, true);

	//Transition from RELOCATION AFTER GETTING HIT
	FsmCreatorHelper(*_afterHitState, &mRepositionAfterHit, _idleState, true);

	//Transition from RELOCATION DUE TO STEERING
	FsmCreatorHelper(*_relocateState, &mRelocateSteering, _idleState, true);

	//Initialize States
	if (_idleState) _idleState->InitState(this);
	if (_chaseState) _chaseState->InitState(this);
	if (_atkState) _atkState->InitState(this);
	if (_waitCDState) _waitCDState->InitWaitState(this, 0);
	if (_fleeState) _fleeState->InitState(this);
	if (_afterHitState) _afterHitState->InitState(this);
	if (_relocateState) _relocateState->InitState(this);

	if (mFSM) mFSM->StartFSM(_idleState);
}

void UG_ArtificierBrain::InitAnimations()
{
	Super::InitAnimations();
	if (!mAnimations) return;
	mAnimations->SetMovementSystem(*mMovementSystem);
}

void UG_ArtificierBrain::Update(const float _deltaTime)
{
	Super::Update(_deltaTime);
	if (mFleeComponent) mFleeComponent->UpdateFleeSystem();
	if (mMovementSystem) mMovementSystem->LookAtPosition();
}

void UG_ArtificierBrain::UpdateBooleans()
{
	Super::UpdateBooleans();
	mPlayerTooFar &= !mNeedToFlee;
	mPlayerTooClose = !mIsInCDAnim && mNeedToFlee && mDetectPlayer;
	mAtPlayerRangeButNotTooClose &= !mNeedToFlee;
}

void UG_ArtificierBrain::UpdateAnimations()
{
	if (!mAnimations || !mOwner) return;
	const IMovingMob* _mobSKM = Cast<IMovingMob>(mOwner);
	if (!_mobSKM) return;
	const float _mobVelocity = _mobSKM->GetMobVelocity();
	mAnimations->SetVelocity(_mobVelocity);
}

void UG_ArtificierBrain::CreateListener()
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

void UG_ArtificierBrain::CreateMovementListener()
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

void UG_ArtificierBrain::CreateSteeringListener()
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

void UG_ArtificierBrain::CreateFleeListener()
{
	if (!mFleeComponent) return;
	mFleeComponent->OnTargetTooClose() += [this]()
	{
		mNeedToFlee = true;
	};
}

void UG_ArtificierBrain::CreateFightListener()
{
	Super::CreateFightListener();
	if (!mFightSystem) return;
	mFightSystem->OnTargetInAttackRange() += [this]()
	{
		if (!mMovementSystem) return;
		mMovementSystem->LockRotateOnPlayer(true);
		mMovementSystem->StopMove();
	};

	mFightSystem->OnTargetNotInAttackRange() += [this]()
	{
		if (mMovementSystem) mMovementSystem->LockRotateOnPlayer(false);
	};
}

void UG_ArtificierBrain::InitData(FDataMob& _data)
{
	Super::InitData(_data);
	if (!mMovementSystem) return;
	mMovementSystem->SetRotationSpeed(_data.GetRotationSpeed());
	if (!mFleeComponent) return;
	FDataArtificier* _dataArtificier = StaticCast<FDataArtificier*>(&_data);
	if (!_dataArtificier) return;
	mFleeComponent->SetFleeDatas(_dataArtificier->GetFleeDatas());
	mMovementSystem->SetDistanceToDodge(_dataArtificier->GetDistanceToDodge());
	mMovementSystem->SetAngleToDodge(_dataArtificier->GetAngleToDodge());
}

bool UG_ArtificierBrain::IsValid() const
{
	return Super::IsValid() && mMovementSystem && mSteeringComponent && mPawnMovementComponent && mFleeComponent;
}

#pragma region Override Brain
void UG_ArtificierBrain::Stun()
{
	Super::Stun();
	if (!mMovementSystem) return;
	mMovementSystem->StopMove();
}
#pragma endregion

