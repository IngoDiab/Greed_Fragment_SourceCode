#include "G_Pickable.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Greed/Manager/GameMode/G_GameMode.h"
#include "Greed/Manager/GameInstance/G_GameInstance.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"

AG_Pickable::AG_Pickable()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

void AG_Pickable::PostInitProperties()
{
	Super::PostInitProperties();
	InitEvents();
}

void AG_Pickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebug();
	if(mInGame) mOnUpdatePickable.Invoke(DeltaTime);
}

void AG_Pickable::BeginPlay()
{
	Super::BeginPlay();
	mOnInitPickable.Invoke();
}

void AG_Pickable::BeginDestroy()
{
	ResetEvents();
	Super::BeginDestroy();
}

void AG_Pickable::InitEvents()
{
	mOnInitPickable += [this]()
	{
		InitPickable();
		mInGame = true;
	};

	mOnUpdatePickable += [this](float DeltaTime)
	{
		UpdateDistance();
		CheckInteraction();
		DropMovement(DeltaTime, mDropPos);
	};
}

void AG_Pickable::ResetEvents()
{
	mOnInitPickable.Clear();
	mOnUpdatePickable.Clear();

	mOnRangeInteraction.Clear();
	mOnRangeInteractionLost.Clear();
}

void AG_Pickable::InitPickable()
{
	mDropPos = GetActorLocation();
	InitItemHandler();
	const APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller) return;
	mPlayer = Cast<AG_MainCharacter>(_controller->GetPawn());
}

void AG_Pickable::InitItemHandler()
{
	/*AG_GameMode* _gm = GetWorld()->GetAuthGameMode<AG_GameMode>();
	if (!_gm) return;
	mLevelItemManager = _gm->LevelItemManager();*/

	UG_GameInstance* _gameInstance = Cast<UG_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!_gameInstance) return;
	mItemHandler = _gameInstance->ItemHandler();

}

void AG_Pickable::UpdateDistance()
{
	if (!mPlayer) return;
	const FVector _playerPos = mPlayer->GetActorLocation();
	mDistance = FVector::Distance(_playerPos, GetActorLocation());
}

bool AG_Pickable::IsAtRange(const float& _range) const
{
	return mDistance <= _range;
}

void AG_Pickable::DropMovement(const float& DeltaTime, const FVector& _targetPos)
{
	const FVector _newPos = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), _targetPos, DeltaTime, mDropSpeed);
	SetActorLocation(_newPos);
}

void AG_Pickable::InitMovementScene(USceneComponent& _compToMove) 
{
	if (!&_compToMove) return;
	mCompBasicLocation = _compToMove.GetRelativeLocation();
	mCompUpOffset = FVector(mCompBasicLocation.X, mCompBasicLocation.Y, mCompBasicLocation.Z + mUpOffset);
}

void AG_Pickable::UpDown(const float& DeltaTime, USceneComponent& _compToMove)
{
	if (!&_compToMove) return;
	const FVector _posToCheck = mMovingUp ? mCompUpOffset : mCompBasicLocation;
	const bool _meshIsAtPos = CompIsAtPos(_posToCheck, _compToMove);
	if (_meshIsAtPos) mMovingUp = !mMovingUp;
	const FVector _currentPos = _compToMove.GetRelativeLocation();
	const FVector _newPos = UKismetMathLibrary::VInterpTo_Constant(_currentPos, _posToCheck, DeltaTime, mUpDownSpeed);
	_compToMove.SetRelativeLocation(_newPos);
}

void AG_Pickable::Rotate(const float& DeltaTime, USceneComponent& _compToRotate)
{
	if (!&_compToRotate) return;
	const FRotator _currentRotator = _compToRotate.GetRelativeRotation();
	float _offsetRotator = _currentRotator.Yaw + DeltaTime * mSpeedRotate;
	_offsetRotator = _offsetRotator >= 360 ? 0 : _offsetRotator;
	const FRotator _newRotator = FRotator(_currentRotator.Pitch, _offsetRotator, _currentRotator.Roll);
	_compToRotate.SetRelativeRotation(_newRotator);
}

bool AG_Pickable::CompIsAtPos(const FVector& _pos, const USceneComponent& _comp) const
{
	if (!&_comp) return false;
	return FVector::Distance(_comp.GetRelativeLocation(), _pos) < 1;
}

void AG_Pickable::CheckInteraction()
{
	const bool _isAtRange = IsAtRange(mRangeInteraction);
	if (_isAtRange) mOnRangeInteraction.Invoke();
	else mOnRangeInteractionLost.Invoke();
}

void AG_Pickable::DrawDebug()
{
	if (!mUseDebugInteraction) return;
	const UWorld* _world = GetWorld();
	if (!_world) return;
	UKismetSystemLibrary::DrawDebugCircle(_world, GetActorLocation(), mRangeInteraction, 360, mColorDebugInteraction, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
	const FVector _pos = GetActorLocation();
	DrawDebugSphere(_world, _pos + mCompUpOffset, 10, 25, FColor::Red);
	DrawDebugSphere(_world, _pos + mCompBasicLocation, 10, 25, FColor::Red);
}


