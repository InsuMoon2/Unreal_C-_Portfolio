#include "Character/CCharacter.h"

#include "Utilities/CHelpers.h"

ACCharacter::ACCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


