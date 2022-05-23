#include "G_Soul.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"

#include "Greed/3C/MainCharacter/G_MainCharacter.h"
#include "Greed/Soulbag/G_SoulbagComponent.h"
#include "Greed/Item/Manager/G_ItemHandler.h"

AG_Soul::AG_Soul()
{
	PrimaryActorTick.bCanEverTick = true;
	mNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System"));
	mNiagaraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AG_Soul::BeginDestroy()
{
	mOnRangeAcquisition.Clear();
	Super::BeginDestroy();
}

void AG_Soul::InitPickable()
{
	Super::InitPickable();
	InitTimerDropping();
	InitMovementScene(*mNiagaraComponent);
	mCurrentSpeed = mSpeed;
}

void AG_Soul::InitEvents()
{
	Super::InitEvents();

	mOnUpdatePickable += [this](float DeltaTime)
	{
		CheckRangeAcquisition();
		MoveToPlayer(DeltaTime);
		DropMovement(DeltaTime, mDropPos);
		UpDown(DeltaTime, *mNiagaraComponent);
		Rotate(DeltaTime, *mNiagaraComponent);
	};
	
	mOnRangeInteraction += [this]()
	{
		mCanMoveToPlayer = true;
		OnSoulMoveBegin();
	};
	
	mOnRangeInteractionLost += [this]()
	{
		mCanMoveToPlayer = false;
	};
	
	mOnRangeAcquisition += [this]()
	{
		OnSoulPickedUp();
		AddToPlayerInventory();
	};
}

void AG_Soul::ResetEvents()
{
	Super::ResetEvents();
	mOnRangeAcquisition.Clear();
}

void AG_Soul::InitTimerDropping()
{
	FTimerHandle _handleFinishDropping;
	FTimerDelegate _delegateFinishDropping;
	_delegateFinishDropping.BindLambda([this]() { mIsDropping = false;});
	GetWorldTimerManager().SetTimer(_handleFinishDropping, _delegateFinishDropping, mTimerDropping, false);
}

void AG_Soul::MoveToPlayer(const float& DeltaTime)
{
	if (mIsDropping || !mCanMoveToPlayer || !mPlayer) return;
	const FVector _posToMove = mPlayer->GetActorLocation();
	mCurrentSpeed += mAcceleration;
	const FVector _newPos = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), _posToMove, DeltaTime, mCurrentSpeed);
	SetActorLocation(_newPos);
	OnSoulMoveInContinue();
}

void AG_Soul::CheckInteraction()
{
	if (mCanMoveToPlayer) return;
	Super::CheckInteraction();
}

void AG_Soul::CheckRangeAcquisition()
{
	const bool _isAtRange = IsAtRange(mRangeAdd);
	if (_isAtRange) mOnRangeAcquisition.Invoke();
}

void AG_Soul::AddToPlayerInventory()
{
	if (!mPlayer) return;
	UG_SoulbagComponent* _soulbag = mPlayer->GetSoulbag();
	if (!_soulbag)return;
	_soulbag->Increase();
	mItemHandler->RemovePickableFromLevelItemManager(*this);
//	if (mLevelItemManager) mLevelItemManager->RemovePickable(*this);
	Destroy();
}

void AG_Soul::DrawDebug()
{
	Super::DrawDebug();
	if (!mUseDebugAcquisition)return;
	UKismetSystemLibrary::DrawDebugCircle(GetWorld(), GetActorLocation(), mRangeAdd, 360, mColorDebugAcquisition, 0, 2, FVector(0, 1, 0), FVector(1, 0, 0));
}
