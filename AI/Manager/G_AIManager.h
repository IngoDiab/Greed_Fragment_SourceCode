#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "G_AIManager.generated.h"

class AG_Mob;

UCLASS()
class GREED_API UG_AIManager : public UObject
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "All Mob in scene"))
	TArray<AG_Mob*> mAllMob;

public:
	FORCEINLINE TArray<AG_Mob*> GetAllMob() const { return mAllMob; };

public:
	void AddMob(AG_Mob& _mob);
	void RemoveMob(AG_Mob& _mob);
	void KillAll();

protected:
	bool Exists(const AG_Mob& _mob) const;
};
