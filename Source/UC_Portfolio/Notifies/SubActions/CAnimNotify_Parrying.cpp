#include "Notifies/SubActions/CAnimNotify_Parrying.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotify_Parrying::GetNotifyName_Implementation() const
{
	return "Off_Parrying";
}

void UCAnimNotify_Parrying::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(character);
	CheckNull(weapon);

	state->OffParryingMode();
}
