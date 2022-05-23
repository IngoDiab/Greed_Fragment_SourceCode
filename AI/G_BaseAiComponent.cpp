#include "G_BaseAiComponent.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"

UG_BaseAiComponent::UG_BaseAiComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UG_BaseAiComponent::BeginPlay()
{
	Super::BeginPlay();
	InitOwner();
}

void UG_BaseAiComponent::InitOwner()
{
	mOwner = Cast<AG_BaseMob>(GetOwner());
}


