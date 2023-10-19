#include "Weapon/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CCamComponent.h"

void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	// Component Setting
	Movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	State = CHelpers::GetComponent<UCStateComponent>(InOwner);
	CameraControl = CHelpers::GetComponent<UCCamComponent>(InOwner);

	
}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (OnEquipmentEquip.IsBound())
		OnEquipmentEquip.Broadcast();

	if (Data.bCanMove == false)
		Movement->Stop();

	if (!!CameraControl)
	{
		if (Data.bUseControlRotation)
			CameraControl->EnableControlRotation();
	}
	
	if (!!Data.Montage)
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);

	else
	{
		// Idle·Î State º¯°æ
		Begin_Equip();
		End_Equip();
	}
		
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();
}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	Movement->Move();
	State->SetIdleMode();
}

void UCEquipment::Unequip_Implementation()
{
	bEquipped = false;

	if(!!CameraControl)
		CameraControl->DisableControlRotation();

	if (OnEquipmentUnequip.IsBound())
		OnEquipmentUnequip.Broadcast();
}
