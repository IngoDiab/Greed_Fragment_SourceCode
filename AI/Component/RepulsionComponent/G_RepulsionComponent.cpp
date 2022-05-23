#include "G_RepulsionComponent.h"

UG_RepulsionComponent::UG_RepulsionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UG_RepulsionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UG_RepulsionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

