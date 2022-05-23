#include "G_AIController.h"
#include "Greed/AI/Component/PathFollowing/G_CustomPathFollowingComponent.h"
#include "Greed/AI/Interfaces/MovingMob/MovingMob.h"

AG_AIController::AG_AIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UG_CustomPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void AG_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	InitSteering();
}

void AG_AIController::InitSteering()
{
	APawn* _pawn = GetPawn();
	if (!_pawn) return;
	UPathFollowingComponent* _pathFollowComp = GetPathFollowingComponent();
	if (!_pathFollowComp) return;
	IMovingMob* _movableMob = Cast<IMovingMob>(_pawn);
	if (!_movableMob) return;
	UG_CustomPathFollowingComponent* _customPathFollowComp = StaticCast<UG_CustomPathFollowingComponent*>(_pathFollowComp);
	if (!_customPathFollowComp) return;
	UG_SteeringComponent* _steeringComp = _movableMob->GetSteeringComponent();
	_customPathFollowComp->SetSteeringComponent(*_steeringComp);
}
