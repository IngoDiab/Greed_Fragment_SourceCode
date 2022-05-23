#include "G_BaseMob.h"

#include "Greed/AI/DataTable/Mob/DataMob.h"
#include "Greed/FSM/FSMComponent/G_FSMComponent.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"
#include "Greed/Skills/Component/G_SkillComponent.h"
#include "Greed/AI/Brain/G_Brain.h"

AG_BaseMob::AG_BaseMob()
{
	CreateAIComponents();
}

void AG_BaseMob::PostInitProperties()
{
	Super::PostInitProperties();
	InitBrain();
}

void AG_BaseMob::BeginDestroy()
{
	ClearAction();
	Super::BeginDestroy();
}

void AG_BaseMob::ChangeTarget(ITarget* _player)
{
	mOnChangeTarget.Broadcast(_player);
}

void AG_BaseMob::ClearAction()
{
	Super::ClearAction();
	mOnChangeTarget.Clear();
}

void AG_BaseMob::CreateAIComponents()
{
	mSkillComponent = CreateDefaultSubobject<UG_SkillComponent>(TEXT("SkillSystem"));
	AddOwnedComponent(mSkillComponent);

	mFSM = CreateDefaultSubobject<UG_FSMComponent>(TEXT("FSM"));
	AddOwnedComponent(mFSM);

	mFightSystem = CreateDefaultSubobject<UG_FightSystemComponent>(TEXT("FightSystem"));
	AddOwnedComponent(mFightSystem);
}

void AG_BaseMob::InitBrain()
{
	if (!mBrain) return;
	mBrain->SetFSMComponent(*mFSM);
	mBrain->SetSkillComponent(*mSkillComponent);
	mBrain->SetFightComponent(*mFightSystem);
}

void AG_BaseMob::InitComponentsWithData(FG_EnemyData& _data)
{
	Super::InitComponentsWithData(_data);
	if (!mBrain) return;
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (!_mobData) return;
	mBrain->InitData(*_mobData);
	mBrain->InitFSM();
}

void AG_BaseMob::FreezeMob(const bool& _freeze)
{
	Super::FreezeMob(_freeze);
	if (!mBrain) return;
	_freeze ? mBrain->Stun() : mBrain->EndStun();
}