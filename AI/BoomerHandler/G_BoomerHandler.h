#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "G_BoomerHandler.generated.h"

class AG_Boomer;

UCLASS(Blueprintable)
class GREED_API UG_BoomerHandler : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UWorld* mWorld = nullptr;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "All Boomers"))
	TArray<AG_Boomer*> mAllBoomers = TArray<AG_Boomer*>();

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Delay Attack"))
	float mDelayAttack = .5f;
	
	UPROPERTY()
	FTimerHandle mDelayAttackHandle;

public:
	FORCEINLINE TArray<AG_Boomer*> GetAllBoomers() const { return mAllBoomers; };
	FORCEINLINE void SetWorld(UWorld& _world) { mWorld = &_world; };

public:
	void AddBoomer(AG_Boomer& _boomer);
	void RemoveBoomer(AG_Boomer& _boomer);
	void BlockAttack(const bool& _block);
	void LatenteAttack();
	void ClearTimeManager(FTimerHandle& _handler);

protected:
	virtual void BeginDestroy() override;
	bool Exists(const AG_Boomer& _boomer) const;

	
};
