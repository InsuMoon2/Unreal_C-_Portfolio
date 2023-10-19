#include "Notifies/AirActions/CAnimNotify_AirMode.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotify_AirMode::GetNotifyName_Implementation() const
{
	return "AirMode";
}

void UCAnimNotify_AirMode::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());
	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(state);

	FVector velocity(0.0f);

	state->OnAirActionMode();
	//character->LaunchCharacter(LaunchZ, false, false);
	character->GetCharacterMovement()->GravityScale = 0.0f;
	character->GetCharacterMovement()->Velocity = velocity;

};
