#include "Components/CCameraControlComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

void UCCameraControlComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACPlayer>(GetOwner());
}

void UCCameraControlComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCCameraControlComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UCCameraControlComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCCameraControlComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

