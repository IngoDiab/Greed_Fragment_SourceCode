#include "G_SoulbagComponent.h"
#include "../Manager/GameInstance/G_GameInstance.h"
#include "../UI/HUD/G_HUD.h"
#include "../UI/Main/G_UI_Main.h"

void UG_SoulbagComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetWorld()) return;
	UG_GameInstance* _gInstance = Cast<UG_GameInstance>(GetWorld()->GetGameInstance());
	if (_gInstance)
		mSpiritbag = _gInstance->Spiritbag();
}

void UG_SoulbagComponent::BeginDestroy()
{
	mOnAmountUpdated.Clear();
	mOnIncreaseAmount.Clear();
	mOnDecreaseAmount.Clear();
	mOnResetAmount.Clear();

	Super::BeginDestroy();
}

void UG_SoulbagComponent::Increase(const int& _amount)
{
	mSoulsAmount += _amount;
	mOnIncreaseAmount.Invoke(_amount);
	mOnAmountUpdated.Invoke(mSoulsAmount);
}

void UG_SoulbagComponent::Decrease(const int& _amount)
{
	mSoulsAmount -= _amount;
	mOnDecreaseAmount.Invoke(_amount);
	mOnAmountUpdated.Invoke(mSoulsAmount);
}

void UG_SoulbagComponent::Reset()
{
	mOnResetAmount.Invoke(mSoulsAmount);
	mSoulsAmount = 0;
	mOnAmountUpdated.Invoke(mSoulsAmount);
}

void UG_SoulbagComponent::ConvertToSpirits()
{
	if (!mSpiritbag) return;
	mSpiritbag->AddSpirits(mSoulsAmount);
	Reset();

	if (!GetWorld()) return;
	AG_HUD* _hud = GetWorld()->GetFirstPlayerController()->GetHUD<AG_HUD>();
	if (!_hud || _hud->GetMainUI()) return;
	_hud->GetMainUI()->DisplaySpirits();
}
