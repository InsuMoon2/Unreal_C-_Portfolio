#include "Weapon/Skills/Wizard/CSkill_Wizard_C.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"

UCSkill_Wizard_C::UCSkill_Wizard_C()
{
}

void UCSkill_Wizard_C::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
}

void UCSkill_Wizard_C::Skill_Pressed(ESkillType InSkillIndex)
{
	Super::Skill_Pressed(ESkillType::Skill3);
}

void UCSkill_Wizard_C::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill3);
}

void UCSkill_Wizard_C::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
}

void UCSkill_Wizard_C::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
}

void UCSkill_Wizard_C::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
}
