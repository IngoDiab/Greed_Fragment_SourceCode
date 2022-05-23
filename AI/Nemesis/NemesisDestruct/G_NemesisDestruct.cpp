#include "G_NemesisDestruct.h"

AG_NemesisDestruct::AG_NemesisDestruct()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AG_NemesisDestruct::BeginDestroy()
{
	mOnEndDestruction.Clear();
	Super::BeginDestroy();
}

void AG_NemesisDestruct::EndDestruct()
{
	mOnEndDestruction.Invoke();
}