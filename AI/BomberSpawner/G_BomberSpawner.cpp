#include "G_BomberSpawner.h"

#include "Greed/Utils/Math/MathUtils.h"
#include "Kismet/KismetMathLibrary.h"

#include "Greed/AI/Boomer/G_Boomer.h"
#include "Greed/AI/Brain/G_Brain.h"
#include "Greed/Manager/FXManager/G_FXManager.h"

AG_BomberSpawner::AG_BomberSpawner()
{
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>("Entity Mesh");
	mMesh->SetupAttachment(RootComponent);
}

void AG_BomberSpawner::BeginDestroy()
{
	if (mWorld)
	{
		FTimerManager& _timerManager = mWorld->GetTimerManager();
		if (_timerManager.TimerExists(mTimerHandle)) _timerManager.ClearTimer(mTimerHandle);
	}
	Super::BeginDestroy();
}

void AG_BomberSpawner::InitMob()
{
	FDataBoomerSpawner* _data = GetDataFromTable<FDataBoomerSpawner>();
	InitStatsFromDatatable(*_data);
	InitComponentsWithData(*_data);
	Super::InitMob();
	if (!mWorld)return;
	mTimerHandle = FTimerHandle();
	FTimerDelegate _delegate = FTimerDelegate();
	_delegate.BindUFunction(this, "SpawnMob");
	mWorld->GetTimerManager().SetTimer(mTimerHandle, _delegate, mDelayForSpawn, true);
}

void AG_BomberSpawner::InitStatsFromDatatable(FG_EnemyData& _data)
{
	Super::InitStatsFromDatatable(_data);
	FDataBoomerSpawner* _spawnerBoomerData = StaticCast<FDataBoomerSpawner*>(&_data);
	if (!_spawnerBoomerData) return;
	mDelayForSpawn = _spawnerBoomerData->GetDelay();
	mRadiusSpawner = _spawnerBoomerData->GetRadiusSpawner();
	mMaxNbBoomer = _spawnerBoomerData->GetMaxNbBoomer();
}

void AG_BomberSpawner::OnDrawGizmos()
{
	Super::OnDrawGizmos();
	if (!mWorld || !ShowDebug()) return;
	DrawDebugCircle(mWorld,GetActorLocation(),mRadiusSpawner,32,FColor::Magenta, false, -1, '\000', 2, FVector(1, 0, 0), FVector(0, 1, 0), false);
}

void AG_BomberSpawner::SpawnMob()
{
	if (!IsValid() || mCurrentNbBoomer >= mMaxNbBoomer)return;
	const FVector _loc = MathUtils::RandomPointInCircle(GetActorLocation(),mRadiusSpawner);
	FTransform _transform = FTransform();
	_transform.SetLocation(_loc);
	_transform.SetRotation(GetActorRotation().Quaternion());
	AG_Boomer* _boomer = mWorld->SpawnActorDeferred<AG_Boomer>(mBoomerPrefab, _transform);
	if (!_boomer) return;
	_transform.SetLocation(_loc);
	_boomer->InitSpawned(this);
	_boomer->FinishSpawning(_transform);
	USkeletalMeshComponent* _boomerMesh = _boomer->GetSkeletalMesh();
	if (_boomerMesh) _boomerMesh->SetRelativeScale3D(FVector::ZeroVector);
	if (mFXManager) mFXManager->SpawnFXMob(G_ETypeMob::BOOMER_SPAWNER, mSpawnBoomerFXName, GetActorLocation(), GetActorRotation());
	mCurrentNbBoomer++;
}

void AG_BomberSpawner::RemoveBoomer()
{
	mCurrentNbBoomer--;
}