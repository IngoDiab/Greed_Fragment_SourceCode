#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class G_ETypeMob : uint8
{
	BOOMER_SPAWNER,
	BOOMER,
	SORCELER,
	TANK,
	SUPPORT,
	ARTIFICIER,
	BOSS,
	OTHER
};
