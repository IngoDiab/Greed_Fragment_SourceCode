#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Greed/Item/Interface/Interactible/Interactible.h"
#include "G_InteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREED_API UG_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

	Action<FVector> mOnAddInteractible = Action<FVector>();
	Action<FVector> mOnRemoveInteractible = Action<FVector>();

	Action<TScriptInterface<IInteractible>> mOnFocusInteractible = Action<TScriptInterface<IInteractible>>();
	Action<TScriptInterface<IInteractible>> mOnLooseInteractible = Action<TScriptInterface<IInteractible>>();

	UPROPERTY(VisibleAnywhere, Category = "Component", meta = (DisplayName = "Owner"))
	AActor* mOwner = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Interactibles", meta = (DisplayName = "All Interactibles"))
	TArray<TScriptInterface<IInteractible>> mAllInteractible;
	
	UPROPERTY(VisibleAnywhere, Category = "Interactibles", meta = (DisplayName = "Current Interactibles"))
	TScriptInterface<IInteractible> mCurrentInteractible = nullptr;

public:
	FORCEINLINE Action<TScriptInterface<IInteractible>>& OnFocusInteractible() { return mOnFocusInteractible; };
	FORCEINLINE Action<TScriptInterface<IInteractible>>& OnLooseInteractible() { return mOnLooseInteractible; };

public:
	FORCEINLINE TScriptInterface<IInteractible> GetCurrentInteractible() const { return mCurrentInteractible; };
	FORCEINLINE void SetCurrentInteractible(const TScriptInterface<IInteractible> _inter) { mCurrentInteractible = _inter; };

public:	
	UG_InteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginDestroy() override;

protected:
	virtual void BeginPlay() override;

private:
	void InitEvents();
	void ResetEvents();
	bool Exists(const TScriptInterface<IInteractible> _inter) const;
	TScriptInterface<IInteractible> GetNearestInteractible();
	void SetNearestInteractible();
	void CalculNearest(TScriptInterface<IInteractible>& _newNearest, float& _dist, const TScriptInterface<IInteractible> _interactible) const;
	void DrawDebugs();

public:
	void Interact();
	void AddToAllInteractible(TScriptInterface<IInteractible> _inter);
	void RemoveFromAllInteractible(TScriptInterface<IInteractible> _inter);
};
