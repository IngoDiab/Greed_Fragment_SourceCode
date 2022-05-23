#include "G_InteractPick.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Greed/Manager/GameInstance/G_GameInstance.h"
#include "Greed/Manager/GameMode/G_GameMode.h"
#include "Greed/Manager/FXManager/G_FXManager.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"
#include "Greed/Interaction/G_InteractionComponent.h"
#include "Greed/Soulbag/G_SoulbagComponent.h"
#include "Greed/UI/Components/G_UI_ComponentBase.h"

void AG_InteractPick::DrawDebug()
{
	if (!mIsInteractible) return;
	Super::DrawDebug();
}


AG_InteractPick::AG_InteractPick()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateComponents();
}

void AG_InteractPick::EnableUI(const bool& _enable)
{
	if (!mUIComponent) return;
	_enable ? mUIComponent->ActiveUI() : mUIComponent->UnactiveUI();
}

FVector AG_InteractPick::GetInteractionLocation() const
{
	return GetActorLocation();
}

void AG_InteractPick::SetLevel(const uint8& _lvl)
{
	mLevel = _lvl;
	mOnLevelChanged.Invoke();
}

void AG_InteractPick::BeginDestroyItem()
{
	mIsInteractible = false;
	FXDestroyItem();
	if (mDelayDestroy <= 0) EndDestroyItem();
	else GetWorld()->GetTimerManager().SetTimer(mDelayDestroyHandler, this, &AG_InteractPick::EndDestroyItem, mDelayDestroy, false);
}

void AG_InteractPick::EndDestroyItem()
{
	FTimerManager& _timeManager = GetWorld()->GetTimerManager();
	if (_timeManager.TimerExists(mDelayDestroyHandler)) _timeManager.ClearTimer(mDelayDestroyHandler);
	Destroy();
}

void AG_InteractPick::BeginDestroy()
{
	mOnInteract.Clear();
	Super::BeginDestroy();
}

void AG_InteractPick::CreateComponents()
{
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	mUIComponent = CreateDefaultSubobject<UG_UI_ComponentBase>(TEXT("UI Component"));
	mUIComponent->SetupAttachment(RootComponent);

	mFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FX Component"));
	mFXComponent->SetupAttachment(RootComponent);
}

void AG_InteractPick::InitPickable()
{
	Super::InitPickable();
	InitItemRefs();
	InitPlayerInteractComp();
	InitMovementScene(*mMesh);
	SpawnPopLevelFX();
	EnableUI(false);
	mOnLevelChanged += ([this] { SpawnPopLevelFX(); });
}

void AG_InteractPick::InitItemRefs()
{
	AG_GameMode* _gm = GetWorld()->GetAuthGameMode<AG_GameMode>();
	if (!_gm) return;
	mFXManager = _gm->FXManager();
}

void AG_InteractPick::InitPlayerInteractComp()
{
	if (!mPlayer) return;
	mPlayerInterComp = mPlayer->GetInteractionComponent();
	mPlayerSoulbag = mPlayer->GetSoulbag();
}

void AG_InteractPick::SpawnPopLevelFX()
{
	if (!mFXComponent || !mFXManager) return;
	if(mMesh) mMesh->SetHiddenInGame(true);
	mIsInteractible = false;
	mFXManager->SpawnFXLoot(mKeyPopFX, mLevel, GetActorLocation(), GetActorRotation());
	FTimerHandle _spawnLootPersistentFXHandle;
	GetWorld()->GetTimerManager().SetTimer(_spawnLootPersistentFXHandle, this, &AG_InteractPick::SpawnPeristentLevelFX, .2f, false);
}

void AG_InteractPick::SpawnPeristentLevelFX()
{
	if (!mFXComponent || !mFXManager) return;
	mFXManager->SpawnFXLoot(mKeyPersistentFX, mLevel, *mFXComponent);
	if (mMesh) mMesh->SetHiddenInGame(false);
	mIsInteractible = true;
}

void AG_InteractPick::InitEvents()
{
	Super::InitEvents();

	mOnUpdatePickable += [this](float DeltaTime)
	{
		UpDown(DeltaTime, *mMesh);
	};

	mOnRangeInteraction += [this]()
	{
		AddToPlayerComponent();
	};

	mOnRangeInteractionLost += [this]()
	{
		RemoveFromPlayerComponent();
	};
}

void AG_InteractPick::ResetEvents()
{
	Super::ResetEvents();
	mOnInteract.Clear();
}

void AG_InteractPick::AddToPlayerComponent()
{
	if (!mPlayerInterComp || !mIsInteractible) return;
	mPlayerInterComp->AddToAllInteractible(this);
}

void AG_InteractPick::RemoveFromPlayerComponent()
{
	if (!mPlayer) return;
	UG_InteractionComponent* _interComp = mPlayer->GetInteractionComponent();
	if (!_interComp) return;
	_interComp->RemoveFromAllInteractible(this);
}

void AG_InteractPick::Interact()
{
	if (!mIsInteractible || !mPlayerSoulbag) return;
	const int _soulsAmount = mPlayerSoulbag->GetSoulsAmount();
	if (_soulsAmount < mPrice) return;
	mOnInteract.Invoke();
	mPlayerSoulbag->Decrease(mPrice);
}
