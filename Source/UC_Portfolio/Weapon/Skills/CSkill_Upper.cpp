#include "Weapon/Skills/CSkill_Upper.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CAttachment.h"
#include "Weapon/DoActions/CDoAction_Combo.h"

void UCSkill_Upper::Skill_Upper()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());
	CheckFalseResult(
		Status->CompareInMana(SkillData.ManaPoint),
		GameUI->ShowLackText(true));

	Hitted.Empty();

	Super::Skill_Upper();

	State->SetActionMode();
	State->OnSkillMode();
	State->OnAirActionMode();

	Owner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SkillData.DoAction(Owner);
}

void UCSkill_Upper::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	//CLog::Print(bhit);
}

void UCSkill_Upper::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	FVector velocity(0,0,600);
	Owner->LaunchCharacter(velocity, false, false);

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkill_Upper::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkill_Upper::OnAttachmentBeginOverlap);
}

void UCSkill_Upper::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();

	if(!!Camera)
		Camera->DisableFixedCamera();
}

void UCSkill_Upper::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* character : Hitted)
	{
		CheckTrue(character == InOther);
	}

	CheckTrue(CHelpers::IsSameTeam(InAttacker, InOther));

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOther);

	CheckTrue(state->IsRollMode());
	CheckTrue(state->IsBackstepMode());

	Hitted.AddUnique(InOther);

	if(ChangeCheck == true)
	{
		// 플레이어는 Profile 변경 X
		ACPlayer* player = Cast<ACPlayer>(InOther);

		if (!player)
		{
			UCapsuleComponent* capsule = InOther->GetCapsuleComponent();
			if (!!capsule)
				capsule->SetCollisionProfileName(TEXT("AirHitted"));
			FName name = InOther->GetCapsuleComponent()->GetCollisionProfileName();
			//CLog::Print(name.ToString());
		}
	}

	ACEnemy_Boss* Boss = Cast<ACEnemy_Boss>(InOther);
	if (!!Boss)
	{
		BossHitData[0].SendDamage(InAttacker, InAttackCauser, Boss);

		return;
	}

	HitData[0].SendDamage(InAttacker, InAttackCauser, InOther);

}

void UCSkill_Upper::OnAttachmentEndCollision()
{
	//bhit = false;

	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(Owner);
	if (!boss)
		Hitted.Empty();
}
