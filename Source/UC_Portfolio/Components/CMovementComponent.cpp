#include "Components/CMovementComponent.h"

#include "CStateComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

bool UCMovementComponent::IsHittedMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsHittedMode();
}

bool UCMovementComponent::IsGuardMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsGuardMode();
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnBossSpeed()
{
	SetSpeed(ESpeedType::BossRun);
}

void UCMovementComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

