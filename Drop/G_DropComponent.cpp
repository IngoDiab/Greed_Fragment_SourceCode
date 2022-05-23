#include "G_DropComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Greed/Item/Item/Pickable/Soul/G_Soul.h"
#include "Greed/Item/Item/Pickable/InteractPickable/WeaponItem/G_WeaponItem.h"
#include "Greed/Manager/GameInstance/G_GameInstance.h"
#include "Greed/Item/Manager/G_ItemHandler.h"

UG_DropComponent::UG_DropComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UG_DropComponent::PostInitProperties()
{
	Super::PostInitProperties();
	mOnUpdateEvenInEditor += [this]()
	{
		DrawDebug();
	};
}

void UG_DropComponent::BeginPlay()
{
	Super::BeginPlay();
	InitItemHandler();
	mInGame = true;
}

void UG_DropComponent::InitItemHandler() 
{
	UG_GameInstance* _gameInstance = GetWorld()->GetGameInstance<UG_GameInstance>();
	if (!_gameInstance) return;
	mItemHandler = _gameInstance->ItemHandler();
}

void UG_DropComponent::BeginDestroy()
{
	mOnUpdateEvenInEditor.Clear();
	mOnUpdate.Clear();
	Super::BeginDestroy();
}

void UG_DropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	mOnUpdateEvenInEditor.Invoke();
	if (mInGame) mOnUpdate.Invoke();
}

void UG_DropComponent::SpawnObjectInRange(UWorld& _world, UClass& _classToSpawn, const FVector& _posSpawn, const FVector& _baseVector)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Soul"));
	if (!mItemHandler) InitItemHandler();
	if (!mItemHandler) return;
	UE_LOG(LogTemp, Warning, TEXT("Found Item Handler"));
	const FVector _posDrop = _posSpawn + (_baseVector * mMaxRange);
	FTransform _transform = FTransform();
	_transform.SetLocation(_posSpawn);
	AG_Pickable* _pick = mItemHandler->SpawnPickableDeferred<AG_Pickable>(_world, _classToSpawn, _transform);
	if (!_pick) return;
	UE_LOG(LogTemp, Warning, TEXT("Finish Spawn Soul"));
	_pick->SetDropPosition(_posDrop);
	_pick->FinishSpawning(_transform);
}

void UG_DropComponent::SpawnObjectInRange(UWorld& _world, UClass& _classToSpawn, const uint8& _level, const FVector& _posSpawn, const FVector& _baseVector)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Item"));
	if (!mItemHandler) InitItemHandler();
	if (!mItemHandler) return;
	UE_LOG(LogTemp, Warning, TEXT("Found Item Handler"));
	const FVector _posDrop = _posSpawn + (_baseVector * mMaxRange);
	FTransform _transform = FTransform();
	_transform.SetLocation(_posSpawn);
	AG_InteractPick* _pick = mItemHandler->SpawnPickableDeferred<AG_InteractPick>(_world, _classToSpawn, _transform);
	if (!_pick) return;
	UE_LOG(LogTemp, Warning, TEXT("Finish Spawn Item"));
	_pick->SetDropPosition(_posDrop);
	_pick->SetLevel(_level);
	mItemHandler->InitItemRandomly(*_pick, _level);
	_pick->FinishSpawning(_transform);
}

UClass* UG_DropComponent::GetRandomItem()
{
	const uint8 _indexItemToSpawn = UKismetMathLibrary::RandomIntegerInRange(0, mItems.Num()-1);
	return mItems[_indexItemToSpawn].Get();
}

void UG_DropComponent::DropSouls(const FVector& _posSpawn, const FVector& _vectorToDropOn, const bool& _onlyOnThisVector)
{
	if (!mDropSouls || !mSoulBP) return;
	const float _clampedDropRate = UKismetMathLibrary::Clamp(mDropRateSouls, 0, 100);
	const bool _dropSouls = UKismetMathLibrary::RandomFloatInRange(0, 100) <= _clampedDropRate;
	if (!_dropSouls) return;

	UWorld* _world = GetWorld();
	if (!_world) return;
	UClass* _classSoul = mSoulBP.Get();
	if (!_classSoul) return;
	const uint8 _nbSouls = UKismetMathLibrary::RandomIntegerInRange(mNbMinSouls, mNbMaxSouls);
	const float _angleForEach = 360.f/_nbSouls;
	const FVector _vectorBase = FVector::VectorPlaneProject(_vectorToDropOn, FVector::UpVector);
	for (uint8 i = 0; i < _nbSouls; i++)
	{
		if (_onlyOnThisVector) SpawnObjectInRange(*_world, *_classSoul, _posSpawn, _vectorBase);
		else
		{
			const FVector _rotatedVectorBase = _vectorBase.RotateAngleAxis(_angleForEach * i, FVector::UpVector);
			SpawnObjectInRange(*_world, *_classSoul, _posSpawn, _rotatedVectorBase);
		}
	}
}

void UG_DropComponent::DropItems(const FVector& _posSpawn, const FVector& _vectorToDropOn, const bool& _onlyOnThisVector, const uint8& _level)
{
	UE_LOG(LogTemp, Warning, TEXT("DropItems"));
	if (!mDropItems || mItems.Num() == 0) return;
	UE_LOG(LogTemp, Warning, TEXT("DropItems : %i"), mItems.Num());
	UWorld* _world = GetWorld();
	if (!_world) return;
	const float _angleForEach = 360.f / mNbItems;
	const FVector _vectorBase = FVector::VectorPlaneProject(_vectorToDropOn, FVector::UpVector);
	for (uint8 i = 0; i < mNbItems; i++) 
	{
		UClass* _classItem = GetRandomItem();
		if (_onlyOnThisVector) SpawnObjectInRange(*_world, *_classItem, _level, _posSpawn, _vectorBase);
		else 
		{
			const FVector _rotatedVectorBase = _vectorBase.RotateAngleAxis(_angleForEach * i, FVector::UpVector);
			SpawnObjectInRange(*_world, *_classItem, _level, _posSpawn, _rotatedVectorBase);
		}
	}
}

void UG_DropComponent::DrawDebug()
{
	if (!mUseDebug) return;
	AActor* _owner = GetOwner();
	if (!_owner) return;
	const FVector& _posDebug = _owner->GetActorLocation();
	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), _posDebug, mMaxRange, 360, mColorMaxRange, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
}

