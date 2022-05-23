#include "NemesisRoomManager.h"

#include "Greed/3C/MainCharacter/G_MainCharacter.h"
#include "Greed/Spawner/SpawnerMobile/G_SpawnerMobile.h"
#include "Greed/AI/BaseMob/G_Mob.h"
#include "Greed/AI/Nemesis/G_Nemesis.h"

ANemesisRoomManager::ANemesisRoomManager()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	mTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	mTrigger->SetupAttachment(RootComponent);
}

void ANemesisRoomManager::BeginPlay()
{
	Super::BeginPlay();
	GetPlayerRef();
	if (mTrigger) mTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANemesisRoomManager::EnterTrigger);
}

void ANemesisRoomManager::BeginDestroy()
{
	if (mTrigger) mTrigger->OnComponentBeginOverlap.RemoveDynamic(this, &ANemesisRoomManager::EnterTrigger);
	if (mPlayerMobileSpawner) 
	{
		mPlayerMobileSpawner->OnMobSpawned().RemoveDynamic(this, &ANemesisRoomManager::AddToMobs);
		mPlayerMobileSpawner->OnMobPreDie().RemoveDynamic(this, &ANemesisRoomManager::RemoveFromMobs);
	}
	mOnBeginBoss.Clear();
	mOnEndBoss.Clear();
	Super::BeginDestroy();
}

void ANemesisRoomManager::GetPlayerRef()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller) return;
	mPlayer = Cast<AG_MainCharacter>(_controller->GetPawn());
	if (!mPlayer) return;
	mPlayerMobileSpawner = mPlayer->GetSpawnerMobile();
	if (!mPlayerMobileSpawner) return;
	mPlayerMobileSpawner->SuspendSpawn(true);
	mPlayerMobileSpawner->OnMobSpawned().AddDynamic(this, &ANemesisRoomManager::AddToMobs);
	mPlayerMobileSpawner->OnMobPreDie().AddDynamic(this, &ANemesisRoomManager::RemoveFromMobs);
	if (!mNemesis) return;
	mNemesis->OnDie() += [this]()
	{
		if (mPlayerMobileSpawner) mPlayerMobileSpawner->ActivateSpawn(false);
		KillAll();
	};

	mNemesis->OnEndDestruction() += [this]()
	{
		mOnEndBoss.Invoke();
	};

	mOnEndBoss += [this]() 
	{
		AG_SpawnerManager::OnFinishAllWave.Invoke();
		CleanArena();
	};
}

void ANemesisRoomManager::AddToMobs(AG_Mob* _mob)
{
	if (!mMobs.Contains(_mob)) mMobs.Add(_mob);
	mPlayerMobileSpawner->SuspendSpawn(!IsUnderLimitMobs());
}

void ANemesisRoomManager::RemoveFromMobs(AG_Mob* _mob)
{
	if (mMobs.Contains(_mob)) mMobs.Remove(_mob);
	mPlayerMobileSpawner->SuspendSpawn(!IsUnderLimitMobs());
}

bool ANemesisRoomManager::IsUnderLimitMobs()
{
	return mMobs.Num() < mLimitMob;
}

void ANemesisRoomManager::KillAll()
{
	const int _count = mMobs.Num();
	if (_count <= 0) return;
	for (int _i = _count - 1; _i >= 0; _i--)
	{
		if (!mMobs.IsValidIndex(_i)) continue;
		AG_Mob* _mob = mMobs[_i];
		if (!_mob || _mob->IsActorBeingDestroyed()) continue;
		_mob->Kill(ETypeDamage::None, true);
	}
	if (mMobs.Num() <= 0) return;
	mMobs.Empty();
}

void ANemesisRoomManager::ActiveBoss(const bool& _activate)
{
	if (!mNemesis) return;
	_activate ? mNemesis->Activate() : mNemesis->Stun();
}

void ANemesisRoomManager::EnterTrigger(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	mOnBeginBoss.Invoke();
	ActiveBoss(true);
	if (mPlayerMobileSpawner) mPlayerMobileSpawner->SuspendSpawn(false);
	if (mTrigger) mTrigger->SetGenerateOverlapEvents(false);
}

void ANemesisRoomManager::CleanArena()
{
	if (mNavModifiers.Num() == 0) return;
	for (AActor* _navModifier : mNavModifiers)
	{
		if (!_navModifier)continue;
		_navModifier->SetActorScale3D(FVector::ZeroVector);
	}
	//UNavigationSystemV1::GetCurrent(GetWorld())->Mesh
}
