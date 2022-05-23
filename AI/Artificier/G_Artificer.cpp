#include "G_Artificer.h"
#include "G_ArtificierBrain.h"
#include "Greed/AI/Component/G_FleeComponent.h"

AG_Artificer::AG_Artificer()
{
	CreateAIComponents();
}

void AG_Artificer::CreateAIComponents()
{
	mCannonMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cannon Mesh");
	if (mEntityMesh) mCannonMesh->AttachToComponent(mEntityMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("Socket_Canon"));

	mFleeComponent = CreateDefaultSubobject<UG_FleeComponent>("Flee Component");
	AddOwnedComponent(mFleeComponent);

	mMovementSystem = CreateDefaultSubobject<UG_MovementComponent>(TEXT("MovementSystem"));
	AddOwnedComponent(mMovementSystem);

	mSteeringComponent = CreateDefaultSubobject<UG_SteeringComponent>("Steering Component");
	AddOwnedComponent(mSteeringComponent);

	mPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement"));
	AddOwnedComponent(mPawnMovementComponent);

	mBrain = CreateDefaultSubobject<UG_ArtificierBrain>("Brain Artificier");
	AddOwnedComponent(mBrain);
}

void AG_Artificer::InitMob()
{
	FDataArtificier* _data = GetDataFromTable<FDataArtificier>();
	InitStatsFromDatatable(*_data);
	InitComponentsWithData(*_data);
	Super::InitMob();
}

void AG_Artificer::InitBrain()
{
	Super::InitBrain();
	if (!mBrain)return;
	UG_ArtificierBrain* _brain = StaticCast<UG_ArtificierBrain*>(mBrain);
	if (!_brain) return;
	_brain->SetMovementComponent(*mMovementSystem);
	_brain->SetFleeComponent(*mFleeComponent);
	_brain->SetSteeringComponent(*mSteeringComponent);
	_brain->SetFloatingPawnComponent(*mPawnMovementComponent);
}

void AG_Artificer::InitStatsFromDatatable(FG_EnemyData& _data)
{
	Super::InitStatsFromDatatable(_data);
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (!_mobData) return;
	mStatsTarget.Speed = _mobData->GetSpeedAtSpawn();
	mStatsTarget.MaxSpeed = _mobData->GetMaxSpeed();
}

void AG_Artificer::InitComponentsWithData(FG_EnemyData& _data)
{
	Super::InitComponentsWithData(_data);
	FDataMob* _mobData = StaticCast<FDataMob*>(&_data);
	if (!_mobData) return;
	if (mPawnMovementComponent) mPawnMovementComponent->MaxSpeed = _mobData->GetSpeedAtSpawn();
	if (mMovementSystem) mMovementSystem->SetFilter(mNavmeshFilter);
	if (mFleeComponent) mFleeComponent->SetFilter(mNavmeshFilter);
}

void AG_Artificer::SlowSpeed(const float& Speed)
{
	const float _speedUpdate = Speed * mStatsTarget.MaxSpeed;
	mStatsTarget.Speed = _speedUpdate <= 0 ? 0 : _speedUpdate;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}

void AG_Artificer::AddSpeed(const float& Speed)
{
	mStatsTarget.Speed = mStatsTarget.Speed + Speed < mStatsTarget.MaxSpeed
		? mStatsTarget.Speed + Speed
		: mStatsTarget.MaxSpeed;
	mPawnMovementComponent->MaxSpeed = mStatsTarget.Speed;
}