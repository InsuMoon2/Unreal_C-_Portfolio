#include "Weapon/AddOns/CSkill_BackStep.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"

UCSkill_BackStep::UCSkill_BackStep()
{
}

void UCSkill_BackStep::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	PlayerController = Owner->GetController<APlayerController>();
	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);
}

void UCSkill_BackStep::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Pressed(InSkillIndex);


}

void UCSkill_BackStep::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(InSkillIndex);

}

void UCSkill_BackStep::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

}

void UCSkill_BackStep::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

}

void UCSkill_BackStep::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();

	bInAction = false;
	Behavior->SetWaitMode();
	Behavior->SetSkillTime(0.0f);

	Movement->Move();
}
