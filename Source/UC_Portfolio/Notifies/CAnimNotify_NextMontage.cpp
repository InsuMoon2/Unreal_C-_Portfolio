#include "Notifies/CAnimNotify_NextMontage.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_NextMontage::GetNotifyName_Implementation() const
{
	return "NextLevel";
}

void UCAnimNotify_NextMontage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	OwnerCharacter->PlayAnimMontage(Montage, PlayRate);
}
