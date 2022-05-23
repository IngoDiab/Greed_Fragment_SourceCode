#include "G_Tank.h"
#include "G_TankBrain.h"

AG_Tank::AG_Tank()
{
	CreateAIComponents();

	mSwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sword Mesh");
	if (mEntityMesh) mSwordMesh->AttachToComponent(mEntityMesh, FAttachmentTransformRules::KeepRelativeTransform, mSwordSocket);
}

void AG_Tank::CreateAIComponents()
{
	mMovementSystem = CreateDefaultSubobject<UG_MovementComponent>(TEXT("MovementSystem"));
	AddOwnedComponent(mMovementSystem);

	mSteeringComponent = CreateDefaultSubobject<UG_SteeringComponent>("Steering Component");
	AddOwnedComponent(mSteeringComponent);

	mPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement"));
	AddOwnedComponent(mPawnMovementComponent);

	mBrain = CreateDefaultSubobject<UG_TankBrain>("Brain Tank");
	AddOwnedComponent(mBrain);
}

void AG_Tank::InitMob()
{
	FDataTank* _data = GetDataFromTable<FDataTank>();
	InitStatsFromDatatable(*_data);
	InitComponentsWithData(*_data);
	Super::InitMob();
}

void AG_Tank::InitBrain()
{
	Super::InitBrain();
	if (!mBrain)return;
	UG_TankBrain* _brain = StaticCast<UG_TankBrain*>(mBrain);
	_brain->SetMovementComponent(*mMovementSystem);
	_brain->SetSteeringComponent(*mSteeringComponent);
	_brain->SetFloatingPawnComponent(*mPawnMovementComponent);
}

void AG_Tank::InitStatsFromDatatable(FG_EnemyData& _data)
{
	Super::InitStatsFromDatatable(_data);
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	mStatsTarget.Speed = _mobData->GetSpeedAtSpawn();
	mStatsTarget.MaxSpeed = _mobData->GetMaxSpeed();
}

void AG_Tank::InitComponentsWithData(FG_EnemyData& _data)
{
	Super::InitComponentsWithData(_data);
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (mPawnMovementComponent) mPawnMovementComponent->MaxSpeed = _mobData->GetSpeedAtSpawn();
	if (mMovementSystem) mMovementSystem->SetFilter(mNavmeshFilter);
}

void AG_Tank::SlowSpeed(const float& Speed)
{
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate <= 0 ? 0 : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}

void AG_Tank::AddSpeed(const float& Speed)
{
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate > mStatsTarget.MaxSpeed ? mStatsTarget.MaxSpeed : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}
