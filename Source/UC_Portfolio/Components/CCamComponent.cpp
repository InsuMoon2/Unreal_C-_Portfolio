#include "Components/CCamComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

void UCCamComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACPlayer>(GetOwner());
}

void UCCamComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCCamComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UCCamComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCCamComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

