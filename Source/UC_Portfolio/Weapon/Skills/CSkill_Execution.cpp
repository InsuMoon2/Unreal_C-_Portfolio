#include "Weapon/Skills/CSkill_Execution.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CAttachment.h"

void UCSkill_Execution::Skill_Execution()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());
	CheckFalse(State->IsExecution());

	Super::Skill_Execution();

	State->SetActionMode();
	State->OnSkillMode();

	UCInGameUIComponent* UI = CHelpers::GetComponent<UCInGameUIComponent>(Owner);
	UI->ShowExecutionText(false);

	SkillData.DoAction(Owner);
}

void UCSkill_Execution::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

}

void UCSkill_Execution::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkill_Execution::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkill_Execution::OnAttachmentBeginOverlap);
}

void UCSkill_Execution::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();

	//HitIndex = 0;
	//Camera->DisableFixedCamera();
}

void UCSkill_Execution::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);
	CheckTrue(CHelpers::IsSameTeam(InAttacker, InOther));

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	Hitted.AddUnique(InOther);

	HitData[0].SendDamage(Owner, InAttackCauser, InOther);

	UCStateComponent* enemy_state = CHelpers::GetComponent<UCStateComponent>(InOther);
	UCCamComponent* cam = CHelpers::GetComponent<UCCamComponent>(InAttacker);

	// Attacker ��ġ ����
	FVector location = InAttacker->GetActorLocation() + InAttacker->GetActorForwardVector() * 120.0f;
	FRotator rotation = (-1.0f * InAttacker->GetActorForwardVector()).Rotation();

	InOther->SetActorLocationAndRotation(location, rotation);
	enemy_state->SetExecutionMode();
	enemy_state->OnExecutionMode();

	State->OffExecutionMode();

	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(InOther);
	if (!!boss)
	{
		CheckNull(Grux_ExecutionMontage);
		boss->PlayAnimMontage(Grux_ExecutionMontage);
	}
	
	else
	{
		CheckNull(ExecutionMontage);
		InOther->PlayAnimMontage(ExecutionMontage);
	}

	//InOther->GetCapsuleComponent()->SetCollisionProfileName(TEXT("DownState"));
}

void UCSkill_Execution::OnAttachmentEndCollision()
{
	Hitted.Empty();

	//HitIndex++;
}
