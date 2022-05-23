#include "G_FXManager.h"
#include "NiagaraComponent.h"

void UG_FXManager::SpawnFXMob(const G_ETypeMob& _typeMob, const FString& _typeFX, const FVector& _position, const FRotator& _rotation)
{
	if (!mFXMob.Contains(_typeMob)) return;
	const FFXByTypeMob& _typesFx = mFXMob[_typeMob];
	const FG_FXData _fxData = _typesFx.GetFXData(_typeFX);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, _fxData.GetFX(), _position + _fxData.GetPositionOffset(), _rotation + _fxData.GetRotationOffset(), _fxData.GetScale());
}

void UG_FXManager::SpawnFXLoot(const FString& _typeFX, const uint8& _levelLoot, const FVector& _position, const FRotator& _rotation)
{
	if (!mFXLoot.Contains(_typeFX)) return;
	const FFXByLevelLoot& _FXByLevel = mFXLoot[_typeFX];
	const FG_FXData _fxData = _FXByLevel.GetFXData(_levelLoot);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, _fxData.GetFX(), _position + _fxData.GetPositionOffset(), _rotation + _fxData.GetRotationOffset(), _fxData.GetScale());
}

void UG_FXManager::SpawnFXLoot(const FString& _typeFX, const uint8& _levelLoot, UNiagaraComponent& _fxComp)
{
	if (!mFXLoot.Contains(_typeFX)) return;
	const FFXByLevelLoot& _FXByLevel = mFXLoot[_typeFX];
	const FG_FXData _fxData = _FXByLevel.GetFXData(_levelLoot);
	_fxComp.SetAsset(_fxData.GetFX());
}
