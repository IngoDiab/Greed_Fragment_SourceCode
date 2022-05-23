#include "G_Brain.h"
#include "Greed/AI/BaseMob/G_BaseMob.h"

#include "Greed/FSM/FSMComponent/G_FSMComponent.h"
#include "Greed/FSM/FSMState/AtkState/G_AtkState.h"
#include "Greed/FSM/FSMState/ChaseState/G_ChaseState.h"
#include "Greed/FSM/FSMState/IdleState/G_IdleState.h"
#include "Greed/FSM/FSMState/WaitState/G_WaitState.h"

#include "Greed/AI/Component/G_MovementComponent.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"
#include "Greed/Skills/Component/G_SkillComponent.h"

#include "Greed/Utils/Debug/Debug.h"

UG_Brain::UG_Brain()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UG_Brain::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (mVisualDebug.IsShowDebug()) OnDrawGizmos();
	Update(DeltaTime);
}

void UG_Brain::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void UG_Brain::Init()
{
	InitOwner();
	if (mSkillComponent) mSkillComponent->InitSkillComponent();
	CreateListener();
}

void UG_Brain::InitOwner()
{
	mOwner = Cast<AG_BaseMob>(GetOwner());
	if (!mOwner) return;
	mOwner->OnDrawGizmosEvent() += [this]() { OnDrawGizmos(); };
}

void UG_Brain::Update(const float _deltaTime)
{
	if (mIsStun || !mFSM || mOwner->IsDead()) return;
	mFSM->OnFSMUpdate.Invoke(_deltaTime);
	UpdateFightSystem();
}

void UG_Brain::UpdateFightSystem()
{
	if (!mFightSystem) return;
	mFightSystem->UpdateFightSystem();
}

void UG_Brain::CreateListener()
{
	CreateFightListener();
}

void UG_Brain::CreateFightListener()
{

}

void UG_Brain::InitData(FDataMob& _data)
{
	if (!mFightSystem) return;
	mFightSystem->SetMinAttackRange(_data.GetMinAttackRange());
	mFightSystem->SetMaxAttackRange(_data.GetMaxAttackRange());
}

void UG_Brain::InitFSM()
{
	
}

void UG_Brain::FsmCreatorHelper(UG_FSMState& _currentState, bool* _condition, UG_FSMState* _nextState, bool _reverse) const
{
	UG_FSMTransition* _transition = NewObject<UG_FSMTransition>();
	_transition->Init(_condition, _nextState, _reverse);
	_currentState.AddTransition(_transition);
}

bool UG_Brain::IsValid() const
{
	return mFSM && mOwner && mFightSystem;
}

void UG_Brain::Stun()
{
	mIsStun = true;
	if (!mSkillComponent) return;
	/*/!\ RESET ONLY SKILL 0 /!\*/
	UG_Skill* _skill = mSkillComponent->GetSkill(0);
	if (!_skill) return;
	_skill->ResetSkill();
}

void UG_Brain::EndStun()
{
	mIsStun = false;
}
#pragma region Override
#pragma region IVisualDebug
bool UG_Brain::ShowDebug() const
{
	return mVisualDebug.IsShowDebug();
}

float UG_Brain::DebugSize() const
{
	return mVisualDebug.DebugSize();
}

float UG_Brain::DebugHeight() const
{
	return mVisualDebug.DebugHeight();
}

FColor UG_Brain::DebugValidColor() const
{
	return mVisualDebug.DebugValidColor();
}

FColor UG_Brain::DebugErrorColor() const
{
	return mVisualDebug.DebugErrorColor();
}

void UG_Brain::OnDrawGizmos()
{
	if (!mOwner || !ShowDebug()) return;
	//DrawDebugCircle(GetWorld(), mOwner->GetActorLocation(), mAtkRange, 360, FColor::Red, false, -1, '\000', 5, FVector(0, 1, 0), FVector(1, 0, 0), false);
}

void UG_Brain::StartFsm() const
{
	mFSM->StartFSM();
}
#pragma endregion IVisualDebug
#pragma endregion Override
