#include "Notifies/AirActions/CAnimNotify_GroundMode.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotify_GroundMode::GetNotifyName_Implementation() const
{
	return "GroundMode";
}

void UCAnimNotify_GroundMode::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());
	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(state);

	//state->OffAirActionMode();
	character->LaunchCharacter(Launch, false, false);
	character->GetCharacterMovement()->GravityScale = 1.0f;
	
}
