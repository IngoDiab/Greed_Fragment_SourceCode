#include "G_Boomer.h"
#include "G_BoomerBrain.h"
#include "Kismet/KismetMathLibrary.h"

#include "Greed/Manager/GameMode/G_GameMode.h"
#include "Greed/AI/BoomerHandler/G_BoomerHandler.h"
#include "Greed/3C/MainCharacter/Components/Dash/G_MC_DashComponent.h"
#include "Greed/Drop/G_DropComponent.h"
#include "Greed/AI/Component/G_DetectionComponent.h"
#include "Greed/AI/BomberSpawner/G_BomberSpawner.h"

AG_Boomer::AG_Boomer()
{
	CreateAIComponents();
}

void AG_Boomer::BeginPlay()
{
	Super::BeginPlay();
	OnDie() += [this]()
	{
		AddToBoomerHandler(false);
		if(mIsSpawned && mSpawner) mSpawner->RemoveBoomer();
	};
}

void AG_Boomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ScaleSpawnAnimation();
}

void AG_Boomer::SpawnAnimation()
{
	if (mIsSpawned) return;
	Super::SpawnAnimation();
}

void AG_Boomer::ScaleSpawnAnimation()
{
	if (!mEntityMesh || !mIsSpawned || mEntityMesh->GetRelativeScale3D() == FVector::OneVector) return;
	const FVector _currentScale = mEntityMesh->GetRelativeScale3D();
	const FVector _newScale = UKismetMathLibrary::VInterpTo_Constant(_currentScale, mMaxGrowth, mWorld->DeltaTimeSeconds, mGrowthSpeed);
	mEntityMesh->SetRelativeScale3D(_newScale);
}

void AG_Boomer::InitBrain()
{
	Super::InitBrain();
	UG_BoomerBrain* _brain = Cast<UG_BoomerBrain>(mBrain);
	if (!_brain) return;
	_brain->SetDashComponent(*mDashComponent);
	_brain->SetDetectionComponent(*mDetectionComponent);
	_brain->SetMovementComponent(*mMovementSystem);
	_brain->SetSteeringComponent(*mSteeringComponent);
	_brain->SetFloatingPawnComponent(*mPawnMovementComponent);
}

void AG_Boomer::CreateAIComponents()
{
	mDashComponent = CreateDefaultSubobject<UG_MC_DashComponent>("Dash Component");
	AddOwnedComponent(mDashComponent);

	mDetectionComponent = CreateDefaultSubobject<UG_DetectionComponent>("Detection Component");
	AddOwnedComponent(mDetectionComponent);

	mMovementSystem = CreateDefaultSubobject<UG_MovementComponent>(TEXT("MovementSystem"));
	AddOwnedComponent(mMovementSystem);

	mSteeringComponent = CreateDefaultSubobject<UG_SteeringComponent>("Steering Component");
	AddOwnedComponent(mSteeringComponent);

	mPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement"));
	AddOwnedComponent(mPawnMovementComponent);

	mBrain = CreateDefaultSubobject<UG_BoomerBrain>("BrainBoomer");
	AddOwnedComponent(mBrain);
}

void AG_Boomer::InitMob()
{
	FDataBoomer* _data = GetDataFromTable<FDataBoomer>();
	InitStatsFromDatatable(*_data);
	InitComponentsWithData(*_data);
	Super::InitMob();
	GetBoomerHandler();
	AddToBoomerHandler(true);
}

void AG_Boomer::InitStatsFromDatatable(FG_EnemyData& _data)
{
	Super::InitStatsFromDatatable(_data);
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (!_mobData) return;
	mStatsTarget.Speed = _mobData->GetSpeedAtSpawn();
	mStatsTarget.MaxSpeed = _mobData->GetMaxSpeed();
}

void AG_Boomer::InitComponentsWithData(FG_EnemyData& _data)
{
	Super::InitComponentsWithData(_data);
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (!_mobData) return;
	if (mPawnMovementComponent) mPawnMovementComponent->MaxSpeed = _mobData->GetSpeedAtSpawn();
	if (mMovementSystem) mMovementSystem->SetFilter(mNavmeshFilter);
}

void AG_Boomer::SlowSpeed(const float& Speed)
{
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate <= 0 ? 0 : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}

void AG_Boomer::AddSpeed(const float& Speed)
{		
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate > mStatsTarget.MaxSpeed ? mStatsTarget.MaxSpeed : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}

void AG_Boomer::GetBoomerHandler()
{
	if (!mWorld) return;
	AG_GameMode* _gameMode = mWorld->GetAuthGameMode<AG_GameMode>();
	if (!_gameMode)return;
	mBoomerHandler = _gameMode->BoomerHandler();
}

void AG_Boomer::AddToBoomerHandler(const bool _add)
{
	if (!mBoomerHandler)return;
	_add ? mBoomerHandler->AddBoomer(*this) : mBoomerHandler->RemoveBoomer(*this);
}

void AG_Boomer::InitSpawned(AG_BomberSpawner* _spawner)
{
	mIsSpawned = true;
	mSpawner = _spawner;
	if (mDropComponent) mDropComponent->SetCanDropSouls(false);
}

void AG_Boomer::BlockAttack(const bool& _block)
{
	if (!mBrain)return;
	UG_BoomerBrain* _brain = StaticCast<UG_BoomerBrain*>(mBrain);
	if (!_brain) return;
	_brain->SetOtherBoomerAttacking(_block);
}

void AG_Boomer::BlockOtherBoomers()
{
	if (!mBoomerHandler)return;
	mBoomerHandler->BlockAttack(true);
}