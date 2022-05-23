#include "G_TankBrain.h"

#include "Greed/AI/Animations/G_AnimBaseMob.h"
#include "Greed/FSM/FSMComponent/G_FSMComponent.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"

#include "Greed/FSM/FSMState/AtkState/G_AtkState.h"
#include "Greed/FSM/FSMState/ChaseState/G_ChaseState.h"
#include "Greed/FSM/FSMState/IdleState/G_IdleState.h"
#include "Greed/FSM/FSMState/WaitCDState/G_WaitCDState.h"
#include "Greed/FSM/FSMState/RelocateState/G_RelocateState.h"

void UG_TankBrain::InitFSM()
{
	//Create States
	UG_IdleState* _idleState = NewObject<UG_IdleState>();
	UG_ChaseState* _chaseState = NewObject<UG_ChaseState>();
	UG_AtkState* _atkState = NewObject<UG_AtkState>();
	UG_WaitCDState* _waitCDState = NewObject<UG_WaitCDState>();
	UG_RelocateState* _relocateState = NewObject<UG_RelocateState>();

	//Transition from IDLE
	FsmCreatorHelper(*_idleState, &mPlayerTooFar, _chaseState);
	FsmCreatorHelper(*_idleState, &mRelocateSteering, _relocateState);
	FsmCreatorHelper(*_idleState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_idleState, &mWaitCooldown, _waitCDState);

	//Transition from CHASE
	FsmCreatorHelper(*_chaseState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_chaseState, &mAtRangeAttack, _waitCDState);

	//Transition from ATTACK
	FsmCreatorHelper(*_atkState, &mRelocateSteering, _relocateState);
	FsmCreatorHelper(*_atkState, &mCDWithoutAnim, _waitCDState);
	FsmCreatorHelper(*_atkState, &mPlayerTooFar, _chaseState);

	//Transition from WAIT COOLDOWN
	FsmCreatorHelper(*_waitCDState, &mRelocateSteering, _relocateState);
	FsmCreatorHelper(*_waitCDState, &mAtPlayerRangeButNotTooClose, _atkState);
	FsmCreatorHelper(*_waitCDState, &mPlayerTooFar, _chaseState);

	//Transition from RELOCATION DUE TO STEERING
	FsmCreatorHelper(*_relocateState, &mRelocateSteering, _idleState, true);
	
	//Initialize States
	if (_idleState) _idleState->InitState(this);
	if (_chaseState) _chaseState->InitState(this);
	if (_atkState) _atkState->InitState(this);
	if (_waitCDState) _waitCDState->InitWaitState(this, 0);
	if (_relocateState) _relocateState->InitState(this);

	//Start FSM on IDLE
	if(mFSM) mFSM->StartFSM(_idleState);
}

void UG_TankBrain::InitAnimations()
{
	Super::InitAnimations();
	if (!mAnimations) return;
	mAnimations->SetMovementSystem(*mMovementSystem);
}

void UG_TankBrain::Update(const float _deltaTime)
{
	Super::Update(_deltaTime);
	if (mMovementSystem) mMovementSystem->LookAtPosition();
}

void UG_TankBrain::UpdateAnimations()
{
	if (!mAnimations || !mOwner) return;
	const IMovingMob* _mobSKM = Cast<IMovingMob>(mOwner);
	if (!_mobSKM) return;
	const float _mobVelocity = _mobSKM->GetMobVelocity();
	mAnimations->SetVelocity(_mobVelocity);
}

void UG_TankBrain::CreateListener()
{
	Super::CreateListener();
	CreateMovementListener();
	CreateSteeringListener();
}

void UG_TankBrain::CreateMovementListener()
{
	if (!mMovementSystem) return;
	mMovementSystem->OnPositionReached() += [this]()
	{
		mMovementSystem->StopMove();
		mPawnMovementComponent->Velocity = FVector::ZeroVector;
	};
}

void UG_TankBrain::CreateSteeringListener()
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

	mSteeringComponent->OnPathUdpatedSteering() += [this](FVector _locationSteering)
	{
		if (!mMovementSystem) return;
		mMovementSystem->RefreshNextPathPoint();
		mMovementSystem->SetPosToLook(_locationSteering);
		//mMovementSystem->SetPosToLook(mMovementSystem->GetNextPathPoint());
		DrawDebugSphere(GetWorld(), _locationSteering, 20, 20, FColor::Cyan);
		mMovementSystem->LookAtPosition();
	};
}

void UG_TankBrain::CreateFightListener()
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

bool UG_TankBrain::IsValid() const
{
	return Super::IsValid() && mMovementSystem && mSteeringComponent && mPawnMovementComponent;
}

#pragma region Override Brain
void UG_TankBrain::InitData(FDataMob& _data)
{
	Super::InitData(_data);
	if (!mMovementSystem) return;
	mMovementSystem->SetRotationSpeed(_data.GetRotationSpeed());
}

void UG_TankBrain::Stun()
{
	Super::Stun();
	if (!mMovementSystem) return;
	mMovementSystem->StopMove();
}
#pragma endregion