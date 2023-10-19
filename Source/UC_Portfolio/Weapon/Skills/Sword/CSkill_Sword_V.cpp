#include "Weapon/Skills/Sword/CSkill_Sword_V.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCSkill_Sword_V::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());
	CheckFalseResult(
		Status->CompareInMana(SkillData.ManaPoint),
		GameUI->ShowLackText(true));

	CheckTrueResult(Skill_CoolTime != 0.0f,
		GameUI->ShowCoolTimeText(true));

	Super::Skill_Pressed(ESkillType::Skill4);

	Skill_CoolTime = SkillData.CoolTime;
	Skill_MaxCoolTime = SkillData.CoolTime;

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);
}

void UCSkill_Sword_V::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill4);
}

void UCSkill_Sword_V::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkill_Sword_V::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkill_Sword_V::OnAttachmentBeginOverlap);

	bMoving = true;

	Start = Owner->GetActorLocation();
	End = Start + Owner->GetActorForwardVector() * Distance;

	if (DrawDebug == EDrawDebugTrace::ForDuration)
	{
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green, true,
			5, 0, 3);
	}

	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator rotation = Owner->GetActorRotation();

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	TArray<FHitResult> hitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);

	UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius, height),
		rotation, objects, false, ignores, DrawDebug, hitResults, true);

	for (const FHitResult& hitResult : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

		if (!!character)
		{
			character->GetCapsuleComponent()->SetCollisionProfileName("Sword_SubAction");

			Overlapped.Add(character);
		}
	}

	FHitResult lineHitResult;
	UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1,
		false, ignores, DrawDebug, lineHitResult, true);

	if (lineHitResult.bBlockingHit)
	{
		FVector direction = (End - Start).GetSafeNormal2D();
		End = lineHitResult.Location - (direction * radius * 2);

		if (DrawDebug == EDrawDebugTrace::ForDuration)
			DrawDebugSphere(Owner->GetWorld(), End, radius * 2, 20, FColor::Magenta, true, 2);
	}

	if (DrawDebug == EDrawDebugTrace::ForDuration)
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Green,
			true, 5, 0, 3);
}

void UCSkill_Sword_V::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	bMoving = false;

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();
	Camera->DisableFixedCamera();

	for (ACharacter* character : Overlapped)
	{
		character->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	}

	Overlapped.Empty();
	Hitted.Empty();

	HitIndex = 0;
}

void UCSkill_Sword_V::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	Skill_CoolTime = Weapon->SetCoolTime(Skill_CoolTime, Skill_MaxCoolTime, InDeltaTime);

	CheckFalse(bMoving);

	FVector location = Owner->GetActorLocation();
	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (location.Equals(End, radius))
	{
		bMoving = false;
		Start = End = Owner->GetActorLocation();

		return;
	}

	FVector direction = (End - Start).GetSafeNormal2D();
	Owner->AddActorWorldOffset(direction * Speed, true);

	
}

void UCSkill_Sword_V::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);
	CheckTrue(CHelpers::IsSameTeam(InAttacker, InOther));

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOther);
	CheckTrue(state->IsDeadMode());

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	ACharacter* Boss = Cast<ACEnemy_Boss>(InOther);
	if (!!Boss)
	{
		Hitted.AddUnique(InOther);
		BossHitDatas[HitIndex].SendDamage(InAttacker, InAttackCauser, Boss);

		return;
	}

	Hitted.AddUnique(InOther);

	CheckTrue(HitIndex >= HitDatas.Num());
	HitDatas[HitIndex].SendDamage(Owner, InAttackCauser, InOther);
}

void UCSkill_Sword_V::OnAttachmentEndCollision()
{
	Hitted.Empty();

	HitIndex++;
}
