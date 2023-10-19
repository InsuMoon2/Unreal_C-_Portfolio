#include "Weapon/Skills/Grux/CSkill_GruxSkill2.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCSkill_GruxSkill2::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Pressed(ESkillType::Skill1);

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);
}

void UCSkill_GruxSkill2::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(InSkillIndex);

}

void UCSkill_GruxSkill2::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

}

void UCSkill_GruxSkill2::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	if (Attachment->OnAttachmentEndCollision.IsBound())
		Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");

	if (Attachment->OnAttachmentBeginOverlap.IsBound())
		Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkill_GruxSkill2::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkill_GruxSkill2::OnAttachmentBeginOverlap);
}

void UCSkill_GruxSkill2::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	//Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	//Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();

	HitIndex = 0;
}

void UCSkill_GruxSkill2::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitIndex >= HitDatas.Num());
	HitDatas[HitIndex].SendDamage(Owner, InAttackCauser, InOther);
}

void UCSkill_GruxSkill2::OnAttachmentEndCollision()
{
	Hitted.Empty();

	HitIndex++;
}
