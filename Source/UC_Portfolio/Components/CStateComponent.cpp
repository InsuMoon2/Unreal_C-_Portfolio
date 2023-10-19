#include "Components/CStateComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

FString UCStateComponent::GetStateTypeAsString()
{
	static const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);

	if (!EnumPtr)
	{
		return FString("Invalid");
	}

	FString StateString = EnumPtr->GetNameStringByValue(static_cast<int64>(GetState()));

	return StateString;
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UCStateComponent::SetBackStepMode()
{
	ChangeType(EStateType::BackStep);
}

void UCStateComponent::SetJumpMode()
{
	ChangeType(EStateType::Jump);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHittedMode()
{
	CheckTrue(IsRollMode());
	CheckTrue(IsBackstepMode());

	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetParkourMode()
{
	ChangeType(EStateType::Parkour);
}

void UCStateComponent::SetIncapacitationMode()
{
	ChangeType(EStateType::Incapacitation);
}

void UCStateComponent::SetExecutionMode()
{
	ChangeType(EStateType::Execution);
}

void UCStateComponent::SetGuardMode()
{
	ChangeType(EStateType::Guard);
}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;
}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}

void UCStateComponent::OnSkillMode()
{
	bInSkillMode = true;
}

void UCStateComponent::OffSkillMode()
{
	bInSkillMode = false;
}

void UCStateComponent::OnAirActionMode()
{
	bInAirActionMode = true;
}

void UCStateComponent::OffAirActionMode()
{
	bInAirActionMode = false;
}

void UCStateComponent::OnExecutionMode()
{
	bExecution = true;
}

void UCStateComponent::OffExecutionMode()
{
	bExecution = false;
}

void UCStateComponent::OnParryingMode()
{
	bParrying = true;
}

void UCStateComponent::OffParryingMode()
{
	bParrying = false;
}

void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}

