#include "Notifies/CAnimNotifyState_Combo.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/DoActions/CDoAction_AirCombo.h"
#include "Weapon/DoActions/CDoAction_Combo.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	UCDoAction_AirCombo* airCombo = Cast<UCDoAction_AirCombo>(weapon->GetAirAction());


	switch (ComboType)
	{
	case EComboType::Combo: 
		CheckNull(weapon->GetDoAction());
		CheckNull(combo);
		combo->EnableCombo();
		break;

	case EComboType::AirCombo:
		CheckNull(weapon->GetAirAction());
		CheckNull(airCombo);
		airCombo->EnableCombo();
		break;
	}

}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	UCDoAction_AirCombo* airCombo = Cast<UCDoAction_AirCombo>(weapon->GetAirAction());

	switch (ComboType)
	{
	case EComboType::Combo: 
		CheckNull(weapon->GetDoAction());
		CheckNull(combo);
		combo->DisableCombo();
		break;

	case EComboType::AirCombo:
		CheckNull(weapon->GetAirAction());
		CheckNull(airCombo);
		airCombo->DisableCombo();
		break;
	}
	

	

	

}
