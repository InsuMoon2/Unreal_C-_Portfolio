#include "Weapon/Skills/Sword/CSkill_Sword_C.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/Attachments/CAttachment_Sword.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoAction.h"

void UCSkill_Sword_C::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());
	CheckFalseResult(
		Status->CompareInMana(SkillData.ManaPoint),
		GameUI->ShowLackText(true));

	CheckTrueResult(Skill_CoolTime != 0.0f,
		GameUI->ShowCoolTimeText(true));

	Super::Skill_Pressed(ESkillType::Skill3);

	Skill_CoolTime = SkillData.CoolTime;
	Skill_MaxCoolTime = SkillData.CoolTime;

	Sword = Cast<ACAttachment_Sword>(Weapon->GetAttachment());
	CheckNull(Sword);

	Sword->ChangeFireMaterial();
	Sword->GetNaiagara()->SetVisibility(true);

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);
}

void UCSkill_Sword_C::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill3);

}

void UCSkill_Sword_C::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Skill_CoolTime = Weapon->SetCoolTime(Skill_CoolTime, Skill_MaxCoolTime, InDeltaTime);
}

void UCSkill_Sword_C::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
}

void UCSkill_Sword_C::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();
	Camera->DisableFixedCamera();

	Sword->GetNaiagara()->SetVisibility(false);
	Sword->ChangeMaterialReset();
}