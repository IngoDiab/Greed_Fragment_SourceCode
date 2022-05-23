#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "G_FXData.generated.h"

USTRUCT()
struct GREED_API FG_FXData
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "FX"))
		UNiagaraSystem* mFX = nullptr;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "FX Position Offset"))
		FVector mPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "FX Rotation Offset"))
		FRotator mRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "FX Scale"))
		FVector mScale = FVector(1);
	
	//UPROPERTY(EditAnywhere, meta = (DisplayName = "Use Mob Rotation"))
		//bool mUseMobRotation = false;

public:
	FORCEINLINE UNiagaraSystem* GetFX() const { return mFX; };
	FORCEINLINE FVector GetPositionOffset() const { return mPosition; };
	FORCEINLINE FRotator GetRotationOffset() const { return mRotation; };
	FORCEINLINE FVector GetScale() const { return mScale; };
	//FORCEINLINE bool UseMobRotation() { return mUseMobRotation; };
};
