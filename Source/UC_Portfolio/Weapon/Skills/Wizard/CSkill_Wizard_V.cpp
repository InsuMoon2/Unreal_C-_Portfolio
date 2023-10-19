#include "Weapon/Skills/Wizard/CSkill_Wizard_V.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"

UCSkill_Wizard_V::UCSkill_Wizard_V()
{
}

void UCSkill_Wizard_V::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
}

void UCSkill_Wizard_V::Skill_Pressed(ESkillType InSkillIndex)
{
	Super::Skill_Pressed(ESkillType::Skill4);
}

void UCSkill_Wizard_V::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill4);
}

void UCSkill_Wizard_V::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
}

void UCSkill_Wizard_V::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
}

void UCSkill_Wizard_V::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
}
