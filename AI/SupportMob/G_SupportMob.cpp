#include "G_SupportMob.h"
#include "G_SupportBrain.h"
#include "Greed/AI/DataTable/Support/DataSupport.h"
#include "Greed/AI/Component/G_MovementComponent.h"
#include "Greed/AI/Component/G_FleeComponent.h"

AG_SupportMob::AG_SupportMob()
{
	CreateAIComponents();

	mBrain = CreateDefaultSubobject<UG_SupportBrain>("Support Brain");
	AddOwnedComponent(mBrain);

	mPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement"));
	AddOwnedComponent(mPawnMovementComponent);
}

void AG_SupportMob::CreateAIComponents()
{
	mMovementSystem = CreateDefaultSubobject<UG_MovementComponent>(TEXT("MovementSystem"));
	AddOwnedComponent(mMovementSystem);
	
	mSteeringComponent = CreateDefaultSubobject<UG_SteeringComponent>("Steering Component");
	AddOwnedComponent(mSteeringComponent);

	mFleeComponent = CreateDefaultSubobject<UG_FleeComponent>("FleeComponent");
	AddOwnedComponent(mFleeComponent);
}

void AG_SupportMob::InitMob()
{
	FDataSupport* _data = GetDataFromTable<FDataSupport>();
	InitStatsFromDatatable(*_data);
	InitComponentsWithData(*_data);
	Super::InitMob();
}

void AG_SupportMob::InitBrain()
{
	Super::InitBrain();
	if (!mBrain) return;
	UG_SupportBrain* _brain = StaticCast<UG_SupportBrain*>(mBrain);
	if (!_brain) return;
	_brain->SetMovementComponent(*mMovementSystem);
	_brain->SetFleeComponent(*mFleeComponent);
	_brain->SetSteeringComponent(*mSteeringComponent);
	_brain->SetFloatingPawnComponent(*mPawnMovementComponent);
}

void AG_SupportMob::InitStatsFromDatatable(FG_EnemyData& _data)
{
	Super::InitStatsFromDatatable(_data);
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (!_mobData) return;
	mStatsTarget.Speed = _mobData->GetSpeedAtSpawn();
	mStatsTarget.MaxSpeed = _mobData->GetMaxSpeed();
}

void AG_SupportMob::InitComponentsWithData(FG_EnemyData& _data)
{
	Super::InitComponentsWithData(_data);
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (!_mobData) return;
	if (mPawnMovementComponent) mPawnMovementComponent->MaxSpeed = _mobData->GetSpeedAtSpawn();
	if (mMovementSystem) mMovementSystem->SetFilter(mNavmeshFilter);
	if (mFleeComponent) mFleeComponent->SetFilter(mNavmeshFilter);
}

void AG_SupportMob::SlowSpeed(const float& Speed)
{
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate <= 0 ? 0 : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}

void AG_SupportMob::AddSpeed(const float& Speed)
{
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate > mStatsTarget.MaxSpeed ? mStatsTarget.MaxSpeed : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}
