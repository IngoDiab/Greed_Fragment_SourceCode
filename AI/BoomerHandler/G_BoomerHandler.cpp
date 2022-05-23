#include "G_BoomerHandler.h"
#include "Greed/AI/Boomer/G_Boomer.h"

void UG_BoomerHandler::BeginDestroy()
{
	ClearTimeManager(mDelayAttackHandle);
	Super::BeginDestroy();
}

void UG_BoomerHandler::AddBoomer(AG_Boomer& _boomer)
{
	if (Exists(_boomer)) return;
	mAllBoomers.Add(&_boomer);
}

void UG_BoomerHandler::RemoveBoomer(AG_Boomer& _boomer)
{
	if (!Exists(_boomer)) return;
	mAllBoomers.Remove(&_boomer);
}

void UG_BoomerHandler::BlockAttack(const bool& _block)
{
	const int _nbBoomer = mAllBoomers.Num();
	if (_nbBoomer == 0) return;
	for (int _i = 0; _i < _nbBoomer; _i++) 
	{
		AG_Boomer* _boomer = mAllBoomers[_i];
		if (!_boomer) continue;
		_boomer->BlockAttack(_block);
	}
	if (!_block) return;
	LatenteAttack();
}

void UG_BoomerHandler::LatenteAttack() 
{
	if (!mWorld) return;
	FTimerManager& _timerManager = mWorld->GetTimerManager();
	FTimerDelegate _delegate;
	_delegate.BindLambda([this]() {BlockAttack(false); });
	_timerManager.SetTimer(mDelayAttackHandle, _delegate, mDelayAttack, false);
}

void UG_BoomerHandler::ClearTimeManager(FTimerHandle& _handler)
{
	if (!mWorld) return;
	FTimerManager& _timeManager = mWorld->GetTimerManager();
	if (_timeManager.TimerExists(_handler)) _timeManager.ClearTimer(_handler);
}

bool UG_BoomerHandler::Exists(const AG_Boomer& _boomer) const
{
	return mAllBoomers.Contains(&_boomer);
}