#include "G_Follower.h"
#include "G_FollowerBrain.h"

#pragma region constructor
AG_Follower::AG_Follower()
{
	mBrain = CreateDefaultSubobject<UG_FollowerBrain>("Brain Follower");
	AddOwnedComponent(mBrain);
}
#pragma endregion constructor

#pragma region UE
void AG_Follower::BeginPlay()
{
	Super::BeginPlay();
}
#pragma endregion UE

