#include "G_LevelItemManager.h"
#include "Greed/Item/Item/Pickable/G_Pickable.h"
#include "Greed/LevelStreaming/StreamingSystem/G_StreamingSystem.h"

UG_LevelItemManager::UG_LevelItemManager() 
{
	AG_StreamingSystem::onCollide += [this](/*FName levelName*/)
	{
		//Debug::LogScreen("Change Room Item", FColor::Purple, 10);
		DestroyPickables();
	};
}

void UG_LevelItemManager::AddPickable(AG_Pickable& _pickable)
{
	if (mAllPickablesInScene.Contains(&_pickable)) return;
	mAllPickablesInScene.Add(&_pickable);
}

void UG_LevelItemManager::RemovePickable(AG_Pickable& _pickable)
{
	if (mAllPickablesInScene.Num() == 0 || !mAllPickablesInScene.Contains(&_pickable)) return;
	mAllPickablesInScene.Remove(&_pickable);
}

void UG_LevelItemManager::DestroyPickables()
{
	//1st version
	/*const int _pickablesInScene = mAllPickablesInScene.Num();
	if (_pickablesInScene == 0) return;
	for (int i = _pickablesInScene - 1; i >= 0; --i)
	{
		AG_Pickable* _pickable = mAllPickablesInScene[i];
		if (!_pickable || _pickable->IsActorBeingDestroyed())continue;
		_pickable->Destroy();
	}
	mAllPickablesInScene.Empty();*/
	
	//2nd version
	const int _count = mAllPickablesInScene.Num();
	if (_count <= 0) return;
	for(int i = _count - 1; i >= 0; i--)
	{
		if(!mAllPickablesInScene.IsValidIndex(i))continue;
		AG_Pickable* _pickable = mAllPickablesInScene[i];
		if(!_pickable || _pickable->IsActorBeingDestroyed()) continue;
		_pickable->Destroy();
	}
	if (mAllPickablesInScene.Num() <= 0) return;
	mAllPickablesInScene.Empty();

	//3rd version
	/*int32 _size = mAllPickablesInScene.Num();
	if (_size <= 0) return;
	for (AG_Pickable* _pickable : mAllPickablesInScene)
	{
		if (!_pickable) continue;
		_pickable->Destroy();
	}
	_size = mAllPickablesInScene.Num();
	if (_size <= 0) return;
	mAllPickablesInScene.Empty();*/
}