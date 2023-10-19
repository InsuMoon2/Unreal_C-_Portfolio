#include "Weapon/Skills/Sword/CSkill_Parrying.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Weapon/CDoAction.h"
#include "Sound/SoundCue.h"
#include "Weapon/CAttachment.h"

void UCSkill_Parrying::Skill_Parrying()
{
	CheckFalse(State->IsGuardMode());
	CheckFalse(State->IsParryingMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Parrying();

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);
}

void UCSkill_Parrying::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

}

void UCSkill_Parrying::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkill_Parrying::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkill_Parrying::OnAttachmentBeginOverlap);

	UGameplayStatics::PlaySound2D(Owner->GetWorld(), Sound);
	SkillData.PlayEffect(Owner->GetWorld(), Owner);

	HitStop(); // 이거 CHelper에 빼서 Static으로 만들기

	State->SetActionMode();
	State->OffParryingMode();
}

void UCSkill_Parrying::End_Skill_Implementation()
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
}

void UCSkill_Parrying::HitStop()
{
	CheckTrue(FMath::IsNearlyZero(StopTime));

	TArray<ACharacter*> characters;
	for (AActor* actor : Owner->GetWorld()->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;

			characters.Add(character);
		}
	}

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			for (ACharacter* character : characters)
				character->CustomTimeDilation = 1.0f;
		});

	Owner->GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void UCSkill_Parrying::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);
	CheckTrue(CHelpers::IsSameTeam(InAttacker, InOther));

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	Hitted.AddUnique(InOther);

	ACEnemy_Boss* Boss = Cast<ACEnemy_Boss>(InOther);
	if (!!Boss)
	{
		Boss_HitData[0].SendDamage(Owner, InAttackCauser, InOther);

		return;
	}

	HitData[0].SendDamage(Owner, InAttackCauser, InOther);
}

void UCSkill_Parrying::OnAttachmentEndCollision()
{
	Hitted.Empty();
}
