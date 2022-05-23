#include "G_CustomPathFollowingComponent.h"

#include "NavigationSystem.h"
#include "Navigation/MetaNavMeshPath.h"

#include "Greed/AI/Component/Steering/G_SteeringComponent.h"
#include "Greed/AI/Controller/G_AIController.h"

void UG_CustomPathFollowingComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* _owner = GetOwner();
	if (!_owner) return;
	mController = StaticCast<AG_AIController*>(_owner);
}

void UG_CustomPathFollowingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FAIRequestID UG_CustomPathFollowingComponent::RequestMove(const FAIMoveRequest& RequestData, FNavPathSharedPtr InPath)
{
	if(!mSteeringComponent || !mController) return Super::RequestMove(RequestData, InPath);

	//Get CurrentPath
	FNavigationPath* _path = InPath.Get();
	if(!_path) return Super::RequestMove(RequestData, InPath);

	//Get CurrentPath as an Array of location
	TArray<FNavPathPoint> _currentPath = _path->GetPathPoints();
	const int _currentPathNbPoints = _currentPath.Num();
	if (_currentPathNbPoints <= 0) return Super::RequestMove(RequestData, InPath);

	//Create new path and add the base location (where AI is located)
	TArray<FVector> _newPath = TArray<FVector>();
	_newPath.Add(_currentPath[0]);

	//Add a possible steering avoidance point then add the next location from the current path if no steering avoidance
	const FVector _locationSteering = PathTargetWithSteering(_currentPath, _currentPathNbPoints);
	if(mUseDebug) DrawDebugSphere(GetWorld(), _locationSteering, 20, 20, FColor::Cyan);
	_newPath.Add(_locationSteering);
	if (_currentPathNbPoints >= 2) _newPath.Add(_currentPath[1].Location);
	mPath = _newPath;
	
	//Repack the newly created path
	FMetaNavMeshPath* _metaNavMeshPath = new FMetaNavMeshPath(mPath, *mController);
	TSharedPtr<FMetaNavMeshPath, ESPMode::ThreadSafe> _metaPathPtr(_metaNavMeshPath);
	InPath = FNavPathSharedPtr(_metaPathPtr);

	//Use the new path
	return Super::RequestMove(RequestData, InPath);
}

/// <summary>
/// Return the point where the AI should go to avoid collision with others mobs
/// </summary>
/// <param name="_currentPath">: path the AI is following</param>
/// <param name="_nbPoints">: number of points in this path</param>
/// <returns></returns>
FVector UG_CustomPathFollowingComponent::PathTargetWithSteering(TArray<FNavPathPoint>& _currentPath, const uint8& _nbPoints)
{
	//Check only one location in path (where the AI is located)
	const FVector _currentPos = _currentPath[0].Location;
	if(_nbPoints == 1) return _currentPos;

	//Check if there's no steering component
	const FVector _currentTargetPos = _currentPath[1].Location;
	if (!mSteeringComponent) return _currentTargetPos;

	//Refresh & get steering force
	//mSteeringComponent->RefreshSteering();
	FVector _currentAvoidance = mSteeringComponent->GetAvoidementForce();
	if (_currentAvoidance.IsZero()) return _currentTargetPos;


	//Calculate the average location between avoidance location & real target location
	_currentAvoidance += (_currentTargetPos - _currentPos).GetSafeNormal();
	_currentAvoidance.Normalize();
	_currentAvoidance *= mSteeringComponent->GetAvoidementMultiplicator();
	_currentAvoidance += _currentPos;

	//Check if steering location is on navmesh
	UNavigationSystemV1* _navigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!_navigationSystem) return _currentTargetPos;
	ANavigationData* _navData = _navigationSystem->GetDefaultNavDataInstance();
	if (!_navData) return _currentTargetPos;
	FSharedConstNavQueryFilter _filter = _navData->GetQueryFilter(mSteeringComponent->GetAvoidementFilter());
	FNavLocation _destLoc;
	const bool _onNavmesh = _navigationSystem->ProjectPointToNavigation(_currentAvoidance, _destLoc, FVector::ZeroVector, _navData, _filter);
	if (!_onNavmesh) return _currentTargetPos;

	return _destLoc;
}

