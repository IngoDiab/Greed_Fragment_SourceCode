#include "G_SkillItem.h"

#include "Kismet/KismetMathLibrary.h"

#include "Greed/Item/Structures/G_SkillItemInfo.h"
#include "Greed/Item/Manager/G_ItemHandler.h"

#include "Greed/Skills/Component/G_SkillComponent.h"
#include "Greed/Interaction/G_InteractionComponent.h"
#include "Greed/3C/MainCharacter/G_MainCharacter.h"

#include "Greed/Manager/Achievements/G_AchievementManager.h"
#include "Greed/Manager/GameInstance/G_GameInstance.h"

void AG_SkillItem::CallAchievements()
{
	UG_GameInstance* _gInstance = Cast<UG_GameInstance>(GetWorld()->GetGameInstance());
	if (!_gInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("MOB : No GI Found"));

		return;
	}
	UG_AchievementManager* _achievementManager = _gInstance->GetAchievementManager();
	if (!_achievementManager)
	{
		UE_LOG(LogTemp, Error, TEXT("MOB : No AchievementManager Found"));

		return;
	}
	_achievementManager->TryAddSkillUsed(mID);
}

void AG_SkillItem::InitEvents()
{
	Super::InitEvents();
	mOnInteract += [this]()
	{
		TryEquipSkill();
	};

	mOnUpdatePickable += [this](float DeltaTime)
	{
		if (!mMesh) return;
		if (!mTargetView) InitViewTarget();
		if (!mTargetView) return;
		LookAt(*mTargetView, *mMesh);
	};
}

void AG_SkillItem::InitPickable()
{
	Super::InitPickable();
	if (!mPlayer)return;
	mPlayerSkillComponent = mPlayer->GetSkillComponent();
}

void AG_SkillItem::InitViewTarget()
{
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	if (!_controller) return;
	mTargetView = _controller->GetViewTarget();
}

void AG_SkillItem::LookAt(const AActor& _actor, USceneComponent& _compToRotate)
{
	FRotator _newRotation = UKismetMathLibrary::FindLookAtRotation(_compToRotate.GetRelativeLocation() + GetActorLocation(), _actor.GetActorLocation());
	_newRotation.Pitch = 0;
	_newRotation.Roll = 0;
	_newRotation.Yaw -= 90;
	_compToRotate.SetWorldRotation(_newRotation);
}

void AG_SkillItem::TryEquipSkill()
{
	if (!mPlayerSkillComponent)return;
	mPlayerSkillComponent->AskForPickUpSkill(*this);
}

void AG_SkillItem::EquipSkill(const int& _slotIndex)
{
	if (!mPlayerSkillComponent || !mPlayerInterComp || !mItemHandler)return;
	const FVector _pickedUpPos = GetActorLocation();
	UG_Skill* _pickedUpSkill = mItemHandler->ItemIntoSkill(*GetWorld(), *mPlayer, mID, mLevel, mEffectsID);

	//Achievement
	CallAchievements();

	mPlayerSkillComponent->PickUpSkill(_slotIndex, _pickedUpSkill, _pickedUpPos);
	mPlayerInterComp->RemoveFromAllInteractible(this);
	mItemHandler->RemovePickableFromLevelItemManager(*this);
	//if(mLevelItemManager) mLevelItemManager->RemovePickable(*this);
	BeginDestroyItem();
}
