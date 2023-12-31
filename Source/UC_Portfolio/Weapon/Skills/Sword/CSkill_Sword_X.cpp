#include "Weapon/Skills/Sword/CSkill_Sword_X.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UCSkill_Sword_X::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	// 마나 부족
	CheckFalseResult(
		Status->CompareInMana(SkillData.ManaPoint),
		GameUI->ShowLackText(true));

	// 쿨타임
	CheckTrueResult(Skill_CoolTime != 0.0f,
		GameUI->ShowCoolTimeText(true));

	Super::Skill_Pressed(ESkillType::Skill2);

	Skill_CoolTime = SkillData.CoolTime;
	Skill_MaxCoolTime = SkillData.CoolTime;

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);
}

void UCSkill_Sword_X::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill2);
}

void UCSkill_Sword_X::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Skill_CoolTime = Weapon->SetCoolTime(Skill_CoolTime, Skill_MaxCoolTime, InDeltaTime);
}

void UCSkill_Sword_X::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

}

void UCSkill_Sword_X::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();
	Camera->DisableFixedCamera();
}
