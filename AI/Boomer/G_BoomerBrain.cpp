#include "G_BoomerBrain.h"
#include "G_Boomer.h"

#include "Greed/AI/Animations/G_AnimBaseMob.h"
#include "Greed/FSM/FSMComponent/G_FSMComponent.h"
#include "Greed/FSM/FSMState/AtkState/G_AtkState.h"
#include "Greed/FSM/FSMState/ChaseState/G_ChaseState.h"
#include "Greed/FSM/FSMState/IdleState/G_IdleState.h"
#include "Greed/FSM/FSMState/WaitCDState/G_WaitCDState.h"
#include "Greed/FSM/FSMState/WaitState/G_WaitState.h"
#include "Greed/AI/Component/G_DetectionComponent.h"
#include "Greed/AI/Component/G_MovementComponent.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"
#include "Greed/Skills/AiSKills/BoomerSkill/G_BoomerSkill.h"

#include "Greed/Utils/Debug/Debug.h"
#include "PatrolState/G_PatrolState.h"

void UG_BoomerBrain::InitFSM()
{
	Super::InitFSM();

	UG_IdleState* _idleState = NewObject<UG_IdleState>();
	UG_ChaseState* _chaseState = NewObject<UG_ChaseState>();
	UG_AtkState* _atkState = NewObject<UG_AtkState>();
	UG_WaitCDState* _waitCDState = NewObject<UG_WaitCDState>();
	UG_PatrolState* _patrolState = NewObject<UG_PatrolState>();
	UG_WaitState* _waitPatrolState = NewObject<UG_WaitState>();
	_waitPatrolState->SetRandomWaitTimer(mMinWaitPatrol, mMaxWaitPatrol);

	FsmCreatorHelper(*_idleState, &mPlayerTooFar, _chaseState);
	FsmCreatorHelper(*_idleState, &mDetectPlayer, _patrolState, true);
	FsmCreatorHelper(*_idleState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_idleState, &mWaitCooldown, _waitCDState);

	FsmCreatorHelper(*_chaseState, &mDetectPlayer, _idleState, true);
	FsmCreatorHelper(*_chaseState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_chaseState, &mAtRangeAttack, _waitCDState);

	FsmCreatorHelper(*_atkState, &mCDWithoutAnim, _waitCDState);
	FsmCreatorHelper(*_atkState, &mPlayerTooFar, _chaseState);

	FsmCreatorHelper(*_waitCDState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_waitCDState, &mPlayerTooFar, _chaseState);

	FsmCreatorHelper(*_patrolState, &mDetectPlayer, _idleState);
	FsmCreatorHelper(*_patrolState, &mWaitPatrol, _waitPatrolState);

	FsmCreatorHelper(*_waitPatrolState, &mWaitPatrol, _patrolState, true);
	FsmCreatorHelper(*_waitPatrolState, &mDetectPlayer, _idleState);

	_idleState->InitState(this);
	_chaseState->InitState(this);
	_atkState->InitState(this);
	_waitCDState->InitWaitState(this, 0);
	_patrolState->InitState(this);
	_waitPatrolState->InitWaitState(this, 0);

	if(mFSM) mFSM->StartFSM(_idleState);
}

void UG_BoomerBrain::InitAnimations()
{
	Super::InitAnimations();
	if (!mAnimations) return;
	mAnimations->SetMovementSystem(*mMovementSystem);
}

void UG_BoomerBrain::Update(const float _deltaTime)
{
	Super::Update(_deltaTime);
	if(mDetectionComponent) mDetectionComponent->UpdateDetection();
	if (mMovementSystem) mMovementSystem->LookAtPosition();
}

void UG_BoomerBrain::UpdateBooleans()
{
	Super::UpdateBooleans();
	mAtPlayerRangeButNotTooClose &= !mOtherBoomerAttacking;
}

void UG_BoomerBrain::UpdateAnimations()
{
	if (!mAnimations || !mOwner) return;
	const IMovingMob* _mobSKM = Cast<IMovingMob>(mOwner);
	if (!_mobSKM) return;
	const float _mobVelocity = _mobSKM->GetMobVelocity();
	mAnimations->SetVelocity(_mobVelocity);
}

void UG_BoomerBrain::CreateListener()
{
	Super::CreateListener();
	CreateMovementListener();
	if (!mDetectionComponent) return;
	mDetectionComponent->OnTargetDetected().AddLambda([this]() 
	{
		mDetectPlayer = true;
	});
}

void UG_BoomerBrain::CreateMovementListener()
{
	if (!mMovementSystem) return;
	mMovementSystem->OnPositionReached() += [this]()
	{
		mWaitPatrol = true;
		mMovementSystem->StopMove();
		mPawnMovementComponent->Velocity = FVector::ZeroVector;
	};
}

void UG_BoomerBrain::CreateFightListener()
{
	Super::CreateFightListener();
	if (!mFightSystem) return;
	mFightSystem->OnAttack() += [this](const uint8& _slotSkill)
	{
		if (!mOwner)return;
		AG_Boomer* _owner = StaticCast<AG_Boomer*>(mOwner);
		if (!_owner)return;
		_owner->BlockOtherBoomers();
	};

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

void UG_BoomerBrain::InitData(FDataMob& _data)
{
	Super::InitData(_data);
	if (!mMovementSystem) return;
	mMovementSystem->SetRotationSpeed(_data.GetRotationSpeed());
	if (!mDetectionComponent) return;
	FDataBoomer* _dataBoomer = StaticCast<FDataBoomer*>(&_data);
	if (!_dataBoomer) return;
	mMinWaitPatrol = _dataBoomer->GetTimeWaitMin();
	mMaxWaitPatrol = _dataBoomer->GetTimeWaitMax();
	mPatrolRange = _dataBoomer->GetPatrolRange();
	mDetectionComponent->SetDetectionRange(_dataBoomer->GetDetectionRange());
	if (!mOwner) return;
	mStartPosition = mOwner->GetActorLocation();
	if (mDashComponent) mDashComponent->SetDashComponent(mOwner->GetCapsule());
}

#pragma region Override Brain
void UG_BoomerBrain::Stun()
{
	Super::Stun();
	if (!mMovementSystem) return;
	mMovementSystem->StopMove();
}
#pragma endregion
