#include "G_BaseMobSM.h"

AG_BaseMobSM::AG_BaseMobSM()
{
	mEntityMesh = CreateDefaultSubobject<UStaticMeshComponent>("Entity Mesh");
	mEntityMesh->SetupAttachment(RootComponent);
}