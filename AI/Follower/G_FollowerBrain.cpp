// Fill out your copyright notice in the Description page of Project Settings.


#include "G_FollowerBrain.h"
#include "Greed/FSM/FSMComponent/G_FSMComponent.h"
#include "Greed/FSM/FSMState/AtkState/G_AtkState.h"
#include "Greed/FSM/FSMState/ChaseState/G_ChaseState.h"
#include "Greed/FSM/FSMState/IdleState/G_IdleState.h"
#include "Greed/FSM/FSMState/WaitState/G_WaitState.h"


#pragma region Override
void UG_FollowerBrain::Update(const float DeltaTime)
{
	Super::Update(DeltaTime);
	//mDetection->UpdateDetection(mAtkRange);
}

void UG_FollowerBrain::InitFSM()
{
	Super::InitFSM();
	/*UG_ChaseState* _chaseState = NewObject<UG_ChaseState>();
	UG_AtkState* _AtkState = NewObject<UG_AtkState>();
	UG_IdleState* _idleState = NewObject<UG_IdleState>();
	FsmCreatorHelper(*_chaseState, &mIsAttacking, _AtkState);
	FsmCreatorHelper(*_idleState, &mIsAttacking, _AtkState);
	FsmCreatorHelper(*_chaseState, &mIdle, _idleState);
	FsmCreatorHelper(*_idleState, &mChasing, _chaseState);
	
	if(mWaitWhileReload)
	{
		UG_WaitState* _waitState = NewObject<UG_WaitState>();
		_waitState->SetWaitTimer(mReloadTime);
		
		FsmCreatorHelper(*_AtkState, &mIsAttacking, _waitState, true);
		FsmCreatorHelper(*_waitState, &mIsWaiting, _idleState, true);
		_waitState->InitState(this);
	}
	else
	{
		FsmCreatorHelper(*_AtkState, &mIsAttacking, _idleState, true);
	}
	

	_chaseState->InitState(this);
	_AtkState->InitState(this);
	_idleState->InitState(this);
	mFSM->SetStartState(_chaseState);*/
}

#pragma endregion Override
