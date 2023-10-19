#include "Components/CAIBehaviorComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UCAIBehaviorComponent::UCAIBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UCAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	Blackboard->SetValueAsEnum(AISkillTypeKey, (uint8)SkillType);
}

EAIStateType UCAIBehaviorComponent::GetAIStateType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}

ACharacter* UCAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}

void UCAIBehaviorComponent::SetTarget(ACharacter* InCharacter)
{
	Blackboard->SetValueAsObject("Target", InCharacter);
}

EAISkillType UCAIBehaviorComponent::GetAISkillType()
{
	return (EAISkillType)Blackboard->GetValueAsEnum(AISkillTypeKey);
}

float UCAIBehaviorComponent::GetDistance()
{
	return Blackboard->GetValueAsFloat(DistanceKey);
}

FVector UCAIBehaviorComponent::GetPatrolLoaction()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}

void UCAIBehaviorComponent::SetPatrolLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(PatrolLocationKey, InLocation);
}

FVector UCAIBehaviorComponent::GetAvoidLocation()
{
	return Blackboard->GetValueAsVector(AvoidLocationKey);
}

float UCAIBehaviorComponent::GetSkillTime()
{
	return Blackboard->GetValueAsFloat(SkillTimeKey);
}

void UCAIBehaviorComponent::SetSkillTime(const float InDeltaTime)
{
	Blackboard->SetValueAsFloat(SkillTimeKey, InDeltaTime);
}

bool UCAIBehaviorComponent::IsWaitMode()
{
	return GetAIStateType() == EAIStateType::Wait;
}

bool UCAIBehaviorComponent::IsApproachMode()
{
	return GetAIStateType() == EAIStateType::Approach;
}

bool UCAIBehaviorComponent::IsActionMode()
{
	return GetAIStateType() == EAIStateType::Action;
}

bool UCAIBehaviorComponent::IsPatrolMode()
{
	return GetAIStateType() == EAIStateType::Patrol;
}

bool UCAIBehaviorComponent::IsHittedMode()
{
	return GetAIStateType() == EAIStateType::Hitted;
}

bool UCAIBehaviorComponent::IsAvoidMode()
{
	return GetAIStateType() == EAIStateType::Avoid;
}

bool UCAIBehaviorComponent::IsDeadMode()
{
	return GetAIStateType() == EAIStateType::Dead;
}

bool UCAIBehaviorComponent::IsSkill1()
{
	return GetAISkillType() == EAISkillType::Skill1;
}

bool UCAIBehaviorComponent::IsSkill2()
{
	return GetAISkillType() == EAISkillType::Skill2;
}

bool UCAIBehaviorComponent::IsSkill3()
{
	return GetAISkillType() == EAISkillType::Skill3;
}

bool UCAIBehaviorComponent::IsSkill4()
{
	return GetAISkillType() == EAISkillType::Skill4;
}

void UCAIBehaviorComponent::SetWaitMode()
{
	ChangeType(EAIStateType::Wait);
}

void UCAIBehaviorComponent::SetApproachMode()
{
	ChangeType(EAIStateType::Approach);
}

void UCAIBehaviorComponent::SetActionMode()
{
	ChangeType(EAIStateType::Action);
}

void UCAIBehaviorComponent::SetPatrolMode()
{
	ChangeType(EAIStateType::Patrol);
}

void UCAIBehaviorComponent::SetHittedMode()
{
	ChangeType(EAIStateType::Hitted);
}

void UCAIBehaviorComponent::SetAvoidMode()
{
	ChangeType(EAIStateType::Avoid);
}

void UCAIBehaviorComponent::SetDeadMode()
{
	ChangeType(EAIStateType::Dead);
}

void UCAIBehaviorComponent::SetSkill_Type(EAISkillType InSkillType)
{
	Blackboard->SetValueAsEnum(AISkillTypeKey, (uint8)InSkillType);
}

void UCAIBehaviorComponent::SetDistance(float InDistance)
{
	Blackboard->SetValueAsFloat(DistanceKey, InDistance);
}

void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetAIStateType();

	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);

	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}
