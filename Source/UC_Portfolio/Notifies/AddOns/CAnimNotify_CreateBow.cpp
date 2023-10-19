#include "Notifies/AddOns/CAnimNotify_CreateBow.h"
#include "Global.h"
#include "Weapon/DoActions/CDoAction_Bow.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CDoAction.h"

FString UCAnimNotify_CreateBow::GetNotifyName_Implementation() const
{
	return "Create_Bow";
}

void UCAnimNotify_CreateBow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
	CheckNull(bow);

	bow->CreateArrow();
}
