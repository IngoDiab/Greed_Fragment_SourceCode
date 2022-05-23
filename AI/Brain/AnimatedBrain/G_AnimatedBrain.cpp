#include "G_AnimatedBrain.h"
#include "Greed/AI/BaseMob/BaseMobSKM/G_BaseMobSKM.h"
#include "Greed/AI/Animations/G_AnimBaseMob.h"

#include "Greed/AI/Component/G_MovementComponent.h"
#include "Greed/AI/Component/G_FightSystemComponent.h"
#include "Greed/Skills/Component/G_SkillComponent.h"

void UG_AnimatedBrain::Init()
{
	Super::Init();
	InitAnimations();
}

void UG_AnimatedBrain::InitOwner()
{
	Super::InitOwner();
	if (!mOwner)return;
	mOwner->OnStun() += [this]()
	{
		if (!mAnimations)return;
		mAnimations->SetIsStun(true);
	};

	mOwner->OnEndStun() += [this]()
	{
		if (!mAnimations)return;
		mAnimations->SetIsStun(false);
	};
}

void UG_AnimatedBrain::InitAnimations()
{
	if (!mOwner) return;
	const AG_BaseMobSKM* _mobSKM = StaticCast<AG_BaseMobSKM*>(mOwner);
	if (!_mobSKM) return;
	const USkeletalMeshComponent* _mesh = _mobSKM->GetSkeletalMesh();
	if (!_mesh) return;
	mAnimations = Cast<UG_AnimBaseMob>(_mesh->GetAnimInstance());
	if (!mAnimations || !mSkillComponent) return;
	mAnimations->InitAttackAnimationsSlots(mSkillComponent->GetAllSKills().Num());
	mAnimations->SetFightSystem(*mFightSystem);
}

void UG_AnimatedBrain::Update(const float _deltaTime)
{
	if (mIsStun || !mFSM || mOwner->IsDead()) return;
	UpdateBooleans();
	Super::Update(_deltaTime);
	UpdateAnimations();
}

void UG_AnimatedBrain::UpdateBooleans()
{
	mIsInCD = mWaitCooldown || mIsInCDAnim;
	mCDWithoutAnim = mWaitCooldown && !mIsInCDAnim;
	mAtRangeAttackAndAim = mAtRangeAttack && mFightSystem && mFightSystem->IsPlayerAhead();
	mAtPlayerRangeButNotTooClose = !mIsInCD && mAtRangeAttackAndAim && mDetectPlayer;
	mPlayerTooFar = !mIsInCDAnim && !mAtRangeAttack && mDetectPlayer;
}

void UG_AnimatedBrain::UpdateAnimations()
{
}

void UG_AnimatedBrain::CreateFightListener()
{
	Super::CreateFightListener();
	if (!mFightSystem) return;
	mFightSystem->OnTargetInAttackRange() += [this]()
	{
		mAtRangeAttack = true;
	};

	mFightSystem->OnTargetNotInAttackRange() += [this]()
	{
		mAtRangeAttack = false;
	};

	mFightSystem->OnAttack() += [this](const uint8& _slotSkill)
	{
		if (mAnimations) mAnimations->ActivateAttackAnimation(_slotSkill, true);
	};

	mFightSystem->OnAttackEnded() += [this](const uint8& _slotSkill)
	{
		mWaitCooldown = true;
		if (mAnimations) mAnimations->ActivateAttackAnimation(_slotSkill, false);
	};

	mFightSystem->OnEndCD() += [this]()
	{
		mWaitCooldown = false;
	};

	mFightSystem->OnBeginCDAnim() += [this]()
	{
		mIsInCDAnim = true;
	};

	mFightSystem->OnEndCDAnim() += [this]()
	{
		mIsInCDAnim = false;
	};
}