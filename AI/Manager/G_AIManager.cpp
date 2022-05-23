#include "G_AIManager.h"
#include "Greed/AI/BaseMob/G_Mob.h"

void UG_AIManager::AddMob(AG_Mob& _mob)
{
	if (Exists(_mob)) return;
	mAllMob.Add(&_mob);
}

void UG_AIManager::RemoveMob(AG_Mob& _mob)
{
	if (!Exists(_mob)) return;
	mAllMob.Remove(&_mob);
}

bool UG_AIManager::Exists(const AG_Mob& _mob) const
{
	return mAllMob.Contains(&_mob);
}

void UG_AIManager::KillAll()
{
	const int _count = mAllMob.Num();
	if (_count <= 0) return;
	for (int _i = _count - 1; _i >= 0; _i--)
	{
		if (!mAllMob.IsValidIndex(_i)) continue;
		AG_Mob* _mob = mAllMob[_i];
		if (!_mob || _mob->IsActorBeingDestroyed()) continue;
		_mob->Kill(ETypeDamage::None, true);
	}
	if (mAllMob.Num() <= 0) return;
	mAllMob.Empty();
}