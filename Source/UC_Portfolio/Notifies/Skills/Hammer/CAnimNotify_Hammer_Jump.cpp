#include "Notifies/Skills/Hammer/CAnimNotify_Hammer_Jump.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/CDoAction.h"

FString UCAnimNotify_Hammer_Jump::GetNotifyName_Implementation() const
{
	return "Hammer_Jump";
}

void UCAnimNotify_Hammer_Jump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);
	OwnerCharacter->GetCharacterMovement()->JumpZVelocity = Jump_Velocity;
	OwnerCharacter->Jump();

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

}
