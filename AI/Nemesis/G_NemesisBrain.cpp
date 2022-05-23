#include "G_NemesisBrain.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"
#include "Greed/Skills/Component/G_SkillComponent.h"

void UG_NemesisBrain::Update(const float _deltaTime)
{
	Super::Update(_deltaTime);
	if (mFightSystem && mOwner && !mOwner->IsDead()) mFightSystem->UseAutoRepeatableSkills();
}

void UG_NemesisBrain::CreateFightListener()
{
	Super::CreateFightListener();
	if (!mFightSystem) return;
	mFightSystem->OnAttack() += [this](const uint8& _slotSkill)
	{
		mFightSystem->UseSkill(_slotSkill);
	};
}

void UG_NemesisBrain::FillSkillPhase(const FG_Phase& _phase)
{
	if (!mSkillComponent) return;
	mSkillComponent->AddSkills(_phase.GetSkillsForPhase());
}

void UG_NemesisBrain::FinishSkills()
{
	if (!mSkillComponent) return;
	mSkillComponent->RemoveSkills();
}