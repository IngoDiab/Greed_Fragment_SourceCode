#include "G_FleeComponent.h"

#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Greed/AI/Brain/G_Brain.h"
#include "Greed/AI/BaseMob/G_BaseMob.h"
#include "Greed/Weapons/Weapon/G_Weapon.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"

void UG_FleeComponent::BeginPlay()
{
	Super::BeginPlay();
	InitTarget();
	mNavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	mOnTargetTooClose += [this]()
	{
		mIsFleeing = true;
	};
	mOnTargetFarEnough += [this]()
	{
		mIsFleeing = false;
	};
}

void UG_FleeComponent::BeginDestroy()
{
	mOnTargetTooClose.Clear();
	mOnTargetFarEnough.Clear();
	Super::BeginDestroy();
}

void UG_FleeComponent::InitOwner()
{
	Super::InitOwner();
	if (!mOwner) return;
	mOwner->OnChangeTarget().AddLambda([this](ITarget* _target)
	{
		mTarget.SetInterface(_target);
		if(_target) RefreshFleeStruct(*_target);
	});
}

void UG_FleeComponent::InitTarget()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller) return;
	ITarget* _target = Cast<ITarget>(_controller->GetPawn());
	if (!_target) return;
	mTarget.SetInterface(_target);
	mTarget.SetObject(_controller->GetPawn());

	FTimerHandle _refresh;
	FTimerDelegate _delegate;
	_delegate.BindLambda([=]
	{
		if(mTarget) RefreshFleeStruct(*mTarget);
	});
	GetWorld()->GetTimerManager().SetTimer(_refresh, _delegate, .1f, false);
}

void UG_FleeComponent::RefreshFleeStruct(const ITarget& _target)
{
	const AG_MainCharacter* _chara = Cast<AG_MainCharacter>(&_target);
	if (!_chara) return;
	UG_Weapon* _weapon = _chara->GetWeapon();
	if (!_weapon) ChooseFleeStruct(ETypeWeapon::NONE);
	else ChooseFleeStruct(_weapon->GetTypeWeapon());
}

void UG_FleeComponent::ChooseFleeStruct(const ETypeWeapon& _typeWeapon)
{
	if (!mFleeDatas.Contains(_typeWeapon))return;
	mCurrentTargetWeapon = _typeWeapon;
	mCurrentFleeStruct = mFleeDatas[_typeWeapon];
}

bool UG_FleeComponent::IsPointOnNavMesh(FVector _pos) const
{
	if (!mNavigationSystem) return false;
	FNavLocation _destLoc;

	ANavigationData* _navData = mNavigationSystem->GetDefaultNavDataInstance();
	if (!_navData) return false;
	FSharedConstNavQueryFilter _filter = _navData->GetQueryFilter(mNavmeshFilterBP);

	const bool _onNavmesh = mNavigationSystem->ProjectPointToNavigation(_pos, _destLoc, FVector::ZeroVector, _navData, _filter);
	if (_onNavmesh) DrawDebugSphere(GetWorld(), _destLoc.Location, 20, 20, FColor::Red);
	return _onNavmesh;
}

void UG_FleeComponent::UpdateFleeSystem()
{
	DrawDebugs();
	if (!mOwner || !mTarget) return;
	const float _distanceFromPlayer = FVector::Dist(mOwner->GetActorLocation(), mTarget->TargetPosition());
	const bool _needToFleeNow = _distanceFromPlayer <= mCurrentFleeStruct.DistanceMinBeforeFlee();
	if(!mIsFleeing && _needToFleeNow) mOnTargetTooClose.Invoke();
	else if(mIsFleeing && !_needToFleeNow) mOnTargetFarEnough.Invoke();
}

FVector UG_FleeComponent::FindFleeingPoint() const
{
	if (!mOwner) return FVector::ZeroVector;
	const FVector _mobPos = mOwner->GetActorLocation();
	if (!mTarget) return _mobPos;
	const FVector _targetPos = mTarget->TargetPosition();

	FVector _targetPosToMobPos = _mobPos - _targetPos;
	_targetPosToMobPos = FVector::VectorPlaneProject(_targetPosToMobPos, FVector::UpVector);
	_targetPosToMobPos.Normalize();
	
	FVector _posToFlee = _targetPos + _targetPosToMobPos * mCurrentFleeStruct.DistanceToFlee();
	if (IsPointOnNavMesh(_posToFlee)) return _posToFlee;
	else return FindBestDirection(_targetPosToMobPos, _targetPos);
}

FVector UG_FleeComponent::FindBestDirection(const FVector& _targetPosToMobPos, const FVector& _targetPos) const
{
	for (int i = 0; i < 90; i++) //CAN CAUSE SOME PROBLEMS
	{
		FVector _posAfterRotation = _targetPos + RotateDirection(_targetPosToMobPos, i*2);
		if (IsPointOnNavMesh(_posAfterRotation)) return _posAfterRotation;

		_posAfterRotation = _targetPos + RotateDirection(_targetPosToMobPos, -i*2);
		if (IsPointOnNavMesh(_posAfterRotation)) return _posAfterRotation;
	}
	return mOwner ? mOwner->GetActorLocation() : FVector::ZeroVector;
}

FVector UG_FleeComponent::RotateDirection(const FVector& _targetPosToMobPos, const int _angle) const
{
	FVector _directionToFleeRight = _targetPosToMobPos.RotateAngleAxis(_angle, FVector::UpVector);
	_directionToFleeRight.Normalize();
	_directionToFleeRight *= mCurrentFleeStruct.DistanceToFlee();
	return _directionToFleeRight;
}

void UG_FleeComponent::DrawDebugs()
{
	if (!mOwner || !mTarget || !mShowDebug) return;
	const FVector _posMobDebug = mOwner->GetActorLocation();
	const FVector _posTargetDebug = mTarget->TargetPosition();
	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), _posMobDebug, mCurrentFleeStruct.DistanceMinBeforeFlee(), 360, FColor::Cyan, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), _posTargetDebug, mCurrentFleeStruct.DistanceToFlee(), 360, FColor::Green, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
}
