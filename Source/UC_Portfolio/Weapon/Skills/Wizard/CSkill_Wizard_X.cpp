#include "Weapon/Skills/Wizard/CSkill_Wizard_X.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"

UCSkill_Wizard_X::UCSkill_Wizard_X()
{
}

void UCSkill_Wizard_X::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
}

void UCSkill_Wizard_X::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Pressed(ESkillType::Skill2);

	State->SetActionMode();
	State->OnSkillMode();
	bInAction = true;

	SkillData.DoAction(Owner);
}

void UCSkill_Wizard_X::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill2);
}

void UCSkill_Wizard_X::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
}

void UCSkill_Wizard_X::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
}

void UCSkill_Wizard_X::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();

	bInAction = false;

	Movement->Move();

	if(!!Camera)
		Camera->DisableFixedCamera();

}