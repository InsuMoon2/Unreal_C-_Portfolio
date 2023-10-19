#include "Weapon/Skills/Hammer/CSkill_Hammer_C.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCSkill_Hammer_C::Skill_Pressed(ESkillType InSkillIndex)
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

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);
}

void UCSkill_Hammer_C::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill3);
}

void UCSkill_Hammer_C::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Skill_CoolTime = Weapon->SetCoolTime(Skill_CoolTime, Skill_MaxCoolTime, InDeltaTime);
}

void UCSkill_Hammer_C::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkill_Hammer_C::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkill_Hammer_C::OnAttachmentBeginOverlap);
}

void UCSkill_Hammer_C::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();
	Camera->DisableFixedCamera();

	HitIndex = 0;
}

void UCSkill_Hammer_C::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOther);
	CheckTrue(state->IsDeadMode());

	CheckTrue(CHelpers::IsSameTeam(InAttacker, InOther));

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	ACharacter* Boss = Cast<ACEnemy_Boss>(InOther);
	if (!!Boss)
	{
		CheckTrue(BossHitDatas.Num() - 1 < HitIndex);
		Hitted.AddUnique(InOther);
		BossHitDatas[HitIndex].SendDamage(InAttacker, InAttackCauser, Boss);

		return;
	}

	CheckTrue(HitIndex >= HitDatas.Num());
	Hitted.AddUnique(InOther);
	HitDatas[HitIndex].SendDamage(Owner, InAttackCauser, InOther);
}

void UCSkill_Hammer_C::OnAttachmentEndCollision()
{
	Hitted.Empty();

	HitIndex++;
}
