#pragma once

#include "CoreMinimal.h"
#include "Greed/AI/BaseMob/G_BaseMob.h"
#include "G_Follower.generated.h"

UCLASS()
class GREED_API AG_Follower : public AG_BaseMob
{
	GENERATED_BODY()

#pragma region constructor
AG_Follower();
#pragma endregion constructor

#pragma region UE
	virtual void BeginPlay() override;
#pragma endregion UE
};
