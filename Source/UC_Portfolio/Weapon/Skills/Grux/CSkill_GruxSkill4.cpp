#include "Weapon/Skills/Grux/CSkill_GruxSkill4.h"
#include "CSkill_Grux_Skill4_Decal.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCSkill_GruxSkill4::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Pressed(ESkillType::Skill1);

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);

	FVector start = (FVector(Owner->GetActorLocation().X, Owner->GetActorLocation().Y, Owner->GetActorLocation().Z - 50.0f));
	FVector forward = Owner->GetActorForwardVector();
	FVector end = ((forward * 1000.0f) + start);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(Pawn);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);

	FHitResult hitResult;

	UKismetSystemLibrary::SphereTraceSingleForObjects(
		Owner->GetWorld(), start, start, 750.0f, ObjectTypes, 
		false, IgnoreActors, EDrawDebugTrace::None, hitResult, true);

	if (hitResult.bBlockingHit)
		TraceCharacter = Cast<ACharacter>(hitResult.GetActor());

}

void UCSkill_GruxSkill4::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(InSkillIndex);

}

void UCSkill_GruxSkill4::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

}

void UCSkill_GruxSkill4::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkill_GruxSkill4::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkill_GruxSkill4::OnAttachmentBeginOverlap);

	MoveTo(TraceCharacter);
}

void UCSkill_GruxSkill4::End_Skill_Implementation()
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

void UCSkill_GruxSkill4::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* character : Hitted)
		CheckTrue(character == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitIndex >= HitDatas.Num());
	HitDatas[HitIndex].SendDamage(Owner, InAttackCauser, InOther);
}

void UCSkill_GruxSkill4::OnAttachmentEndCollision()
{
	Hitted.Empty();

	HitIndex++;
}

void UCSkill_GruxSkill4::MoveTo(ACharacter* InTarget)
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	FVector StartLocation = (Owner->GetActorLocation() - InTarget->GetActorLocation()).GetSafeNormal2D();
	FRotator StartRotation = Owner->GetActorRotation();

	float distance = Owner->GetDistanceTo(InTarget);
	//FVector location = InTarget->GetActorLocation()
	//					+ InTarget->GetActorForwardVector() * 200.0f;
	FVector location = Owner->GetActorLocation() + (StartLocation * -(distance - 100.0f));

	UKismetSystemLibrary::MoveComponentTo(
		Owner->GetCapsuleComponent(),
		location, StartRotation, true, true,
		0.5f, false, EMoveComponentAction::Type::Move, 
		LatentInfo);

	FVector DecalLocation = FVector(location.X, location.Y, location.Z - decalZ);

	ACSkill_Grux_Skill4_Decal* decal = 
		Owner->GetWorld()->SpawnActor<ACSkill_Grux_Skill4_Decal>(
			Decal, DecalLocation, StartRotation);

	decal->DecalIncrease();

}

