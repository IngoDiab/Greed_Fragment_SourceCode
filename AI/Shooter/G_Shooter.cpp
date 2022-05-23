#include "G_Shooter.h"
#include "G_ShooterBrain.h"
#include "Greed/AI/Component/Steering/G_SteeringComponent.h"

AG_Shooter::AG_Shooter()
{
	CreateAIComponents();
}

void AG_Shooter::CreateAIComponents()
{
	mFleeComponent = CreateDefaultSubobject<UG_FleeComponent>("Flee Component");
	AddOwnedComponent(mFleeComponent);
	
	mMovementSystem = CreateDefaultSubobject<UG_MovementComponent>(TEXT("MovementSystem"));
	AddOwnedComponent(mMovementSystem);

	mSteeringComponent = CreateDefaultSubobject<UG_SteeringComponent>("Steering Component");
	AddOwnedComponent(mSteeringComponent);

	mPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement"));
	AddOwnedComponent(mPawnMovementComponent);

	mBrain = CreateDefaultSubobject<UG_ShooterBrain>("Brain Shooter");
	AddOwnedComponent(mBrain);
}

void AG_Shooter::InitMob()
{
	FDataShooter* _data = GetDataFromTable<FDataShooter>();
	if (!_data)return;
	InitStatsFromDatatable(*_data);
	InitComponentsWithData(*_data);
	Super::InitMob();
}

void AG_Shooter::InitBrain()
{
	Super::InitBrain();
	if (!mBrain)return;
	UG_ShooterBrain* _brain = StaticCast<UG_ShooterBrain*>(mBrain);
	if (!_brain) return;
	_brain->SetMovementComponent(*mMovementSystem);
	_brain->SetFleeComponent(*mFleeComponent);
	_brain->SetSteeringComponent(*mSteeringComponent);
	_brain->SetFloatingPawnComponent(*mPawnMovementComponent);
}

void AG_Shooter::InitStatsFromDatatable(FG_EnemyData& _data)
{
	Super::InitStatsFromDatatable(_data);
	if (!FDataMob::StaticStruct()->IsChildOf(_data.StaticStruct())) return;
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	mStatsTarget.Speed = _mobData->GetSpeedAtSpawn();
	mStatsTarget.MaxSpeed = _mobData->GetMaxSpeed();
}

void AG_Shooter::InitComponentsWithData(FG_EnemyData& _data)
{
	Super::InitComponentsWithData(_data);
	if (!FDataMob::StaticStruct()->IsChildOf(_data.StaticStruct())) return;
	//CAST
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (!_mobData) return;
	if (mPawnMovementComponent) mPawnMovementComponent->MaxSpeed = _mobData->GetSpeedAtSpawn();
	if (mMovementSystem) mMovementSystem->SetFilter(mNavmeshFilter);
	if (mFleeComponent) mFleeComponent->SetFilter(mNavmeshFilter);
}

void AG_Shooter::SlowSpeed(const float& Speed)
{
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate <= 0 ? 0 : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}

void AG_Shooter::AddSpeed(const float& Speed)
{
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate > mStatsTarget.MaxSpeed ? mStatsTarget.MaxSpeed : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}