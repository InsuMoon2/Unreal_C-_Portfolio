#include "Notifies/AddOns/CAnimNotify_BackStep.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotify_BackStep::GetNotifyName_Implementation() const
{
	return "Launch";
}

void UCAnimNotify_BackStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());
	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(state);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	FVector EndLocation = character->GetActorLocation() + character->GetActorForwardVector() * -700.0f;
	FRotator StartRotation = character->GetActorRotation();

	UKismetSystemLibrary::MoveComponentTo(
		character->GetCapsuleComponent(), EndLocation, StartRotation, true, true, 0.5f, false,
		EMoveComponentAction::Type::Move, LatentInfo);
}
