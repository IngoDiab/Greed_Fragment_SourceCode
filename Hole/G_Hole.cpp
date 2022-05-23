#include "G_Hole.h"

#include "Components/BoxComponent.h"

#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"
#include "Greed/AI/Component/RepulsionComponent/G_RepulsionComponent.h"

AG_Hole::AG_Hole()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	mBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	mBoxCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	mEndBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("End Box Collider"));
	mEndBoxCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	mRepulsionComponent = CreateDefaultSubobject<UG_RepulsionComponent>(TEXT("Repulsion Component"));
	AddOwnedComponent(mRepulsionComponent);
}

void AG_Hole::BeginPlay()
{
	Super::BeginPlay();
	AddListener();
}

void AG_Hole::AddListener() 
{
	if (!mBoxCollider || !mEndBoxCollider) return;
	mBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AG_Hole::EnterTrigger);
	mEndBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AG_Hole::ExitTrigger);
}

void AG_Hole::EnterTrigger(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (!mRepulsionActive || !_otherActor) return;
	//Get mob's steering component
	UG_SteeringComponent* _steeringComponent = GetSteering(*_otherActor);
	if (!_steeringComponent) return;

	//Add its repulsor component to mob's steering
	_steeringComponent->Add<UG_RepulsionComponent>(*mRepulsionComponent,_steeringComponent->GetRepulsiveActorsComponent());
}

void AG_Hole::ExitTrigger(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
	if (!mRepulsionActive || !_otherActor) return;

	//Get mob's steering component
	UG_SteeringComponent* _steeringComponent = GetSteering(*_otherActor);
	if (!_steeringComponent) return;

	//Remove its repulsor component from mob's steering
	_steeringComponent->Remove<UG_RepulsionComponent>(*mRepulsionComponent, _steeringComponent->GetRepulsiveActorsComponent());
}

UG_SteeringComponent* AG_Hole::GetSteering(AActor& _actor) const
{
	//Check/cast actor into IMoving mob
	IMovingMob* _mob = Cast<IMovingMob>(&_actor);
	if (!_mob) return nullptr;

	//Check if mob's type is affected by repulsion
	if (!mTypesMobsAffected.Contains(_mob->GetTypeMob())) return nullptr;

	//Get mob's steering component
	return _mob->GetSteeringComponent();
}