#include "Weapon/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "CSkill.h"
#include "CSubAction.h"
#include "CWeaponData.h"
#include "Character/CPlayer.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	Attachment_Class = ACAttachment::StaticClass();
	Equipment_Class = UCEquipment::StaticClass();

}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner, class UCWeaponData** OutWeaponData)
{
	ACAttachment* attachment = nullptr;
	if (!!Attachment_Class)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(Attachment_Class, params);
	}

	UCEquipment* equipment = nullptr;
	if (!!Equipment_Class)
	{
		equipment = NewObject<UCEquipment>(this, Equipment_Class);
		equipment->BeginPlay(InOwner, EquipmentData);

		if (!!attachment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}

	// doAction
	UCDoAction* doAction = nullptr;
	if (!!DoAction_Class)
	{
		doAction = NewObject<UCDoAction>(this, DoAction_Class);
		doAction->BeginPlay(attachment, equipment, InOwner, DoActionDatas, HitDatas, GuardHitDatas, Boss_HitDatas, Boss_Air_HitDatas);

		if (!!attachment)
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);
		}
		// Bow
		if (!!equipment)
		{
			equipment->OnEquipmentEquip.AddDynamic(doAction, &UCDoAction::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(doAction, &UCDoAction::OnUnequip);
		}
	}
	// subAction
	UCSubAction* subAction = nullptr;
	if (!!SubAction_Class)
	{
		subAction = NewObject<UCSubAction>(this, SubAction_Class);
		subAction->BeginPlay(InOwner, attachment, doAction);
	}

	// airaction
	UCDoAction* airaction = nullptr;
	if (!!AirAction_Class)
	{
		airaction = NewObject<UCDoAction>(this, AirAction_Class);
		airaction->BeginPlay(attachment, equipment, InOwner, Air_ActionDatas, Air_HitDatas, GuardHitDatas, Boss_HitDatas, Boss_Air_HitDatas);
	}

	// Skill
	Skills.Empty();
	TArray<class UCSkill*> skills;
	for (int32 i = 0; i < Skill_MAX; i++)
	{
		if (!!Skill_Class[i])
		{
			skills.Emplace(NewObject<UCSkill>(this, Skill_Class[i]));

			if (!!skills[i])
				skills[i]->BeginPlay(InOwner, attachment, doAction);
		}
	}

	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->Attachment = attachment;
	(*OutWeaponData)->Equipment = equipment;
	(*OutWeaponData)->DoAction = doAction;
	(*OutWeaponData)->SubAction = subAction;
	(*OutWeaponData)->AirAction = airaction;
	(*OutWeaponData)->Skills = skills;
}

#if WITH_EDITOR
void UCWeaponAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	// 게임 모듈 쪽이니까 Super를 사용해도 된다.
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	CheckTrue(FApp::IsGame());

	// 새로고침을 할 조건을 만들어주었다.
	bool bRefresh = false;

	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("DoActionDatas") == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("HitDatas") == 0;
	bRefresh |= PropertyChangedEvent.GetPropertyName().Compare("AirActionDatas") == 0;

	if (bRefresh)
	{
		bool bCheck = false;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear;
		bCheck |= PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate;

		if (bCheck)
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			// DetailView가 다룰 수 있는 최상위 클래스 IDetailsView
			TSharedPtr<IDetailsView> detailsView = prop.FindDetailView("WeaponAssetEditorDetailsView");

			if (detailsView.IsValid())
				detailsView->ForceRefresh();
		}
	}
}
#endif // WITH_EDITOR