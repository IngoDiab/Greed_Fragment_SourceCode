#include "G_AnimBaseMob.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"
#include "Greed/AI/Component/G_MovementComponent.h"

void UG_AnimBaseMob::InitAttackAnimationsSlots(const uint8& _nbSlots)
{
	for (uint8 _slot = 0; _slot < _nbSlots; _slot++) mAttacksAnimations.Add(false);
}

void UG_AnimBaseMob::TriggerAnimAttack(const uint8& _slotSkill)
{
	if (!mFightComp) return;
	mFightComp->UseSkill(_slotSkill);
}

void UG_AnimBaseMob::LockMovements(const bool& _canMove, const bool& _canRotate) 
{
	if (!mMoveComp) return;
	mMoveComp->SetCanMove(_canMove);
	mMoveComp->SetCanRotate(_canRotate);
}

void UG_AnimBaseMob::BeginAnimCooldown()
{
	if (!mFightComp) return;
	mFightComp->OnBeginCDAnim().Invoke();
}

void UG_AnimBaseMob::EndAnimCooldown()
{
	if (!mFightComp) return;
	mFightComp->OnEndCDAnim().Invoke();
}

float UG_AnimBaseMob::GetStateWeight(FName _stateMachineName, FName _stateName)
{
	if (_stateMachineName == "" || _stateName == "") return 0;
    FAnimNode_StateMachine* _stateMachine = GetStateMachineInstanceFromName(_stateMachineName);
    if (!_stateMachine) return 0;
    IAnimClassInterface* _animClassInterface = IAnimClassInterface::GetFromClass(GetClass());
    if (!_animClassInterface) return 0;
    const FBakedAnimationStateMachine* _stateMachineDescription = GetMachineDescription(_animClassInterface, _stateMachine);
    if (!_stateMachineDescription) return 0;
    int _index = _stateMachineDescription->FindStateIndex(_stateName);
    if (_index == INDEX_NONE) return 0;
    return _stateMachine->GetStateWeight(_index);
}