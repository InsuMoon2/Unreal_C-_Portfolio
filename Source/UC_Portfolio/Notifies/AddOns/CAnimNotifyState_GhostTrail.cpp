#include "Notifies/AddOns/CAnimNotifyState_GhostTrail.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/AddOns/CGhostTrail.h"

FString UCAnimNotifyState_GhostTrail::GetNotifyName_Implementation() const
{
	return "Ghost_Trail";
}

void UCAnimNotifyState_GhostTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, OwnerCharacter);

}

void UCAnimNotifyState_GhostTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	CheckNull(GhostTrail);
	GhostTrail->Destroy();
}
