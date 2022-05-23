#include "G_InteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

UG_InteractionComponent::UG_InteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UG_InteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetNearestInteractible();
	DrawDebugs();
}

void UG_InteractionComponent::BeginDestroy()
{
	ResetEvents();
	Super::BeginDestroy();
}

void UG_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	mOwner = GetOwner();
	InitEvents();
}

void UG_InteractionComponent::InitEvents()
{
	mOnFocusInteractible += [this](TScriptInterface<IInteractible> _interactible)
	{
		if (!_interactible) return;
		_interactible->EnableUI(true);
	};

	mOnLooseInteractible += [this](TScriptInterface<IInteractible> _interactible)
	{
		if (!_interactible) return;
		_interactible->EnableUI(false);
	};
}

void UG_InteractionComponent::ResetEvents()
{
	mOnAddInteractible.Clear();
	mOnRemoveInteractible.Clear();

	mOnFocusInteractible.Clear();
	mOnLooseInteractible.Clear();
}

bool UG_InteractionComponent::Exists(const TScriptInterface<IInteractible> _inter) const
{
	if (mAllInteractible.Num() <= 0) return false;
	for (TScriptInterface<IInteractible> _interactible : mAllInteractible)
		if (_inter == _interactible) return true;
	return false;
}

TScriptInterface<IInteractible> UG_InteractionComponent::GetNearestInteractible()
{
	if (!mOwner || mAllInteractible.Num() <= 0) return nullptr;
	TScriptInterface<IInteractible> _newNearest = nullptr;
	float _minDist = TNumericLimits<float>::Max();
	for (TScriptInterface<IInteractible> _interactible : mAllInteractible)
		CalculNearest(_newNearest, _minDist, _interactible);
	return _newNearest;
}

void UG_InteractionComponent::SetNearestInteractible()
{
	TScriptInterface<IInteractible> _newNearest = GetNearestInteractible();
	if (_newNearest && mCurrentInteractible == _newNearest) return;
	if (mCurrentInteractible) mOnLooseInteractible.Invoke(mCurrentInteractible);
	if (_newNearest) mOnFocusInteractible.Invoke(_newNearest);
	mCurrentInteractible = _newNearest;
}

void UG_InteractionComponent::CalculNearest(TScriptInterface<IInteractible>& _newNearest, float& _minDist, const TScriptInterface<IInteractible> _interactible) const
{
	if (!mOwner || !_interactible) return;
	const float _squaredDist = FVector::DistSquared(mOwner->GetActorLocation(), _interactible->GetInteractionLocation());
	if (_squaredDist >= _minDist) return;
	_minDist = _squaredDist;
	_newNearest = _interactible;
}

void UG_InteractionComponent::DrawDebugs()
{
	if (!mCurrentInteractible) return;
	DrawDebugBox(GetWorld(), mCurrentInteractible->GetInteractionLocation(), FVector(50), FColor::Purple);
}

void UG_InteractionComponent::Interact()
{
	if (!mCurrentInteractible) return;
	mCurrentInteractible->Interact();
}

void UG_InteractionComponent::AddToAllInteractible(TScriptInterface<IInteractible> _inter)
{
	if (Exists(_inter)) return;
	mOnAddInteractible(_inter->GetInteractionLocation());
	mAllInteractible.Add(_inter);
}

void UG_InteractionComponent::RemoveFromAllInteractible(TScriptInterface<IInteractible> _inter)
{
	if (!Exists(_inter)) return;
	mOnRemoveInteractible(_inter->GetInteractionLocation());
	mAllInteractible.Remove(_inter);
}

