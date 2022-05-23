#include "G_FightSystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Greed/AI/Brain/G_Brain.h"
#include "Greed/Skills/Component/G_SkillComponent.h"
#include "Greed/Utils/Debug/Debug.h"

#pragma region constructor
UG_FightSystemComponent::UG_FightSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UG_FightSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	InitTarget();
	InitSkill();
	mOnTargetInAttackRange += [this]()
	{
		mPlayerInRange = true;
	};

	mOnTargetNotInAttackRange += [this]()
	{
		mPlayerInRange = false;
	};

	mOnAttack += [this](const uint8& _slot)
	{
		mIsAttacking = true;
	};
	
	mOnAttackEnded += [this](const uint8& _slot)
	{
		mIsAttacking = false;
	};
}

void UG_FightSystemComponent::BeginDestroy()
{
	mOnTargetInAttackRange.Clear();
	mOnTargetNotInAttackRange.Clear();
	mOnAttack.Clear();
	mOnAttackEnded.Clear();
	mOnEndCD.Clear();
	mOnBeginCDAnim.Clear();
	mOnEndCDAnim.Clear();
	Super::BeginDestroy();
}

void UG_FightSystemComponent::InitOwner()
{
	Super::InitOwner();
	if (!mOwner) return;
	mOwner->OnChangeTarget().AddLambda([this](ITarget* _target)
	{
		if (!_target)return;
		mTarget.SetInterface(_target);
		mTarget.SetObject(_target->_getUObject());
	});
}

void UG_FightSystemComponent::InitTarget()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller) return;
	ITarget* _target = Cast<ITarget>(_controller->GetPawn());
	if (!_target) return;
	mTarget.SetInterface(_target);
	mTarget.SetObject(_controller->GetPawn());
}

void UG_FightSystemComponent::InitSkill()
{
	if (!mOwner) return;
	UG_Brain* _brain = mOwner->GetBrain();
	if (!_brain) return;
	mSkillComponent = _brain->GetSkillComponent();
}

void UG_FightSystemComponent::UpdateFightSystem()
{
	if (mUseDebug) DrawDebugs();
	const bool _playerIsNowInRange = CheckAttackRange(mMinAttackRange) && !IsVisionObstructed();
	const bool _playerIsNotInRangeAnymore = !CheckAttackRange(mMaxAttackRange) || IsVisionObstructed();
	if (mTarget && !mPlayerInRange && _playerIsNowInRange) mOnTargetInAttackRange.Invoke();
	else if (mPlayerInRange && (_playerIsNotInRangeAnymore || !mTarget)) mOnTargetNotInAttackRange.Invoke();
}

void UG_FightSystemComponent::UseAutoRepeatableSkills()
{
	if (mAutoRepeatableSkillsSlots.Num() == 0 || mIsAttacking) return;
	for (uint8 _i = 0; _i < mAutoRepeatableSkillsSlots.Num(); _i++)
	{
		int _skillSlot = mAutoRepeatableSkillsSlots[_i];
		UG_Skill* _skill = mSkillComponent->GetSkill(_skillSlot);
		if (!_skill || !_skill->GetSkillStats().GetCanAttack())continue;
		Attack(_skillSlot);
	}
}

bool UG_FightSystemComponent::CheckAttackRange(const float& _attackRange) const
{
	if (!mOwner || !mTarget) return false;
	return FVector::Dist(mOwner->GetActorLocation(), mTarget->TargetPosition()) <= _attackRange;
}

bool UG_FightSystemComponent::IsVisionObstructed() const
{
	if (!mNeedToSeePlayer) return false;
	if (!mOwner || !mTarget) return true;
	FHitResult _result;
	const FVector _mobPos = mOwner->GetActorLocation();
	const FVector _targetPos = mTarget->TargetPosition();
	const FVector _directionMobToTarget = (_targetPos - _mobPos).GetSafeNormal();
	bool _dontSeePlayer = false;
	if(!mUseSphereCast) _dontSeePlayer = UKismetSystemLibrary::LineTraceSingle(GetWorld(), _mobPos, _targetPos, ETraceTypeQuery::TraceTypeQuery7, false, TArray<AActor*>(), mUseDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, _result, true);
	else _dontSeePlayer = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), _mobPos, _targetPos, mRadiusSphereCast, ETraceTypeQuery::TraceTypeQuery7, false, TArray<AActor*>(), mUseDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, _result, true);
	return _dontSeePlayer;
}

bool UG_FightSystemComponent::IsPlayerAhead() const
{
	if (!mNeedPlayerAhead) return true;
	if (!mTarget || !mOwner) return false;
	const FVector _AIToTarget = mTarget->TargetPosition() - mOwner->GetActorLocation();
	const FVector _projectAIToTarget = FVector::VectorPlaneProject(_AIToTarget, FVector::UpVector).GetSafeNormal();
	const FVector _forward = mOwner->GetActorForwardVector();
	const float _dotProduct = FVector::DotProduct(_projectAIToTarget, _forward);
	return _dotProduct >= mTargetAheadAdjustement;
}

void UG_FightSystemComponent::Attack(const uint8& _slotSkill)
{
	if (mIsAttacking || !mSkillComponent) return;
	UG_Skill* _skillToUse = mSkillComponent->GetSkill(_slotSkill);
	if (!_skillToUse) return;
	mOnAttack.Invoke(_slotSkill);
}

void UG_FightSystemComponent::EndAttack(const uint8& _slotSkill)
{
	if (!mIsAttacking || !mSkillComponent) return;
	UG_Skill* _skillToUse = mSkillComponent->GetSkill(_slotSkill);
	if (!_skillToUse) return;
	mOnAttackEnded.Invoke(_slotSkill);
}

void UG_FightSystemComponent::UseSkill(const uint8& _slotSkill) 
{
	if (!mSkillComponent) return;
	UG_Skill* _skillToUse = mSkillComponent->GetSkill(_slotSkill);
	if (!_skillToUse) return;
	_skillToUse->Use();
	_skillToUse->SkillRelease();
	//NEED NO CD FOR AUTOREAPATABLE
	mLastSkillUsed = _skillToUse;
	EndAttack(_slotSkill);
}

bool UG_FightSystemComponent::MobInCD() const
{
	if (!mLastSkillUsed) return false;
	return !mLastSkillUsed->GetSkillStats().GetCanAttack();
}

void UG_FightSystemComponent::DrawDebugs() const 
{
	if (!mOwner)return;
	const FVector _posMobDebug = mOwner->GetActorLocation();
	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), _posMobDebug, mMinAttackRange, 360, FColor::Red, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), _posMobDebug, mMaxAttackRange, 360, FColor::Yellow, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
}
