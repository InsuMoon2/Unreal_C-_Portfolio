#include "Weapon/CDoAction.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "GameFramework/Character.h"
#include "Components/CWeaponComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"

UCDoAction::UCDoAction()
{
	
}

void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas, const TArray<FHitData>& InGuardHitDatas,
	const TArray<FHitData>& InBossHitDatas, const TArray<FHitData>& InBossAirHitDatas)
{
	// Character, World
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();
	Attachment = InAttachment;

	// Component
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	CameraControl = CHelpers::GetComponent<UCCamComponent>(OwnerCharacter);
	Montage = CHelpers::GetComponent<UCMontagesComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
	GuardHitDatas = InGuardHitDatas;
	BossHitDatas = InBossHitDatas;
	Boss_AirHitDatas = InBossAirHitDatas;
}

void UCDoAction::DoAction()
{
	bInAction = true;

	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;

	State->SetIdleMode();

	Movement->Move();

	if (!!CameraControl)
		CameraControl->DisableFixedCamera();
	
}
