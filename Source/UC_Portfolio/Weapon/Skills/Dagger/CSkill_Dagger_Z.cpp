#include "Weapon/Skills/Dagger/CSkill_Dagger_Z.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCSkill_Dagger_Z::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Pressed(ESkillType::Skill1);

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);
}

void UCSkill_Dagger_Z::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill1);
}

void UCSkill_Dagger_Z::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
}

void UCSkill_Dagger_Z::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
}

void UCSkill_Dagger_Z::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();
	Camera->DisableFixedCamera();
}

