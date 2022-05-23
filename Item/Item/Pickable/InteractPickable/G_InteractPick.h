#pragma once

#include "CoreMinimal.h"
#include "Greed/Item/Item/Pickable/G_Pickable.h"
#include "Greed/Item/Interface/Interactible/Interactible.h"
#include "Greed/Effects/Effect/EffectDT/G_EffectDT.h"
#include "Greed/Item/Enum/G_ItemType.h"
#include "G_InteractPick.generated.h"

class UG_ItemHandler;
class UG_FXManager;
class UG_InteractionComponent;
class UG_SoulbagComponent;
class UG_UI_ComponentBase;

UCLASS()
class GREED_API AG_InteractPick : public AG_Pickable, public IInteractible
{
	GENERATED_BODY()

protected:
	Action<> mOnLevelChanged = Action<>();
	Action<> mOnInteract = Action<>();

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (DisplayName = "FX component"))
	UNiagaraComponent* mFXComponent = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Key Pop"))
	FString mKeyPopFX = "Pop";

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Key Persistent"))
	FString mKeyPersistentFX = "Persistent";
	
	UPROPERTY()
	UG_FXManager* mFXManager = nullptr;
	
	UPROPERTY()
	UG_InteractionComponent* mPlayerInterComp = nullptr;

	UPROPERTY()
	UG_SoulbagComponent* mPlayerSoulbag = nullptr;

	UPROPERTY(VisibleAnywhere)
	UG_UI_ComponentBase* mUIComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (DisplayName = "Is Interactible"))
	bool mIsInteractible = true;

	UPROPERTY(EditAnywhere, Category = "Info", meta = (DisplayName = "Item Level"))
	uint8 mLevel = 0;

	UPROPERTY(EditAnywhere, Category = "Info", meta = (DisplayName = "Item Price"))
	int mPrice = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (DisplayName = "Mesh"))
	UStaticMeshComponent* mMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Info", meta = (DisplayName = "Item Type"))
	G_ItemType mType = G_ItemType::OTHER;

	UPROPERTY(EditAnywhere, Category = "Info", meta = (DisplayName = "ID Effects"))
	TArray<uint8> mEffectsID;

	UPROPERTY(Category = "Settings", EditAnywhere, meta = (DisplayName = "Delay Destroy After Picked Up"))
	float mDelayDestroy = .2f;

	UPROPERTY()
	FTimerHandle mDelayDestroyHandler;
	
public:
	FORCEINLINE virtual bool IsInteractible() const {return mIsInteractible;};

	FORCEINLINE UStaticMeshComponent* GetMesh() const { return mMesh; };

	FORCEINLINE G_ItemType GetType() const { return mType; };

	FORCEINLINE TArray<uint8> GetEffectsID() const { return mEffectsID; };
	FORCEINLINE void SetEffectsID(const TArray<uint8> _effectsID) { mEffectsID = _effectsID; };
	FORCEINLINE void AddEffectID(uint8 _effectsID) { mEffectsID.Add(_effectsID); };

	FORCEINLINE uint8 GetLevel() const { return mLevel; };
	void SetLevel(const uint8& _lvl);

	FORCEINLINE UG_UI_ComponentBase* UIComp() { return mUIComponent; };

public:
	FORCEINLINE virtual Action<>* OnInteract() { return &mOnInteract; };

protected: 
	virtual void BeginDestroy() override;
	virtual void CreateComponents();
	virtual void InitPickable() override;
	void InitItemRefs();
	virtual void InitPlayerInteractComp();
	void SpawnPopLevelFX();
	void SpawnPeristentLevelFX();
	virtual void InitEvents() override;
	virtual void ResetEvents() override;

	void AddToPlayerComponent();
	void RemoveFromPlayerComponent();
	/// <summary>
	/// Draw Debugs
	/// </summary>
	virtual void DrawDebug() override;

	virtual void BeginDestroyItem();
	UFUNCTION(BlueprintImplementableEvent)
	void FXDestroyItem();
	virtual void EndDestroyItem();

public:
	AG_InteractPick();
	virtual void EnableUI(const bool& _enable) override;
	virtual FVector GetInteractionLocation() const override;
	virtual void Interact();
};
