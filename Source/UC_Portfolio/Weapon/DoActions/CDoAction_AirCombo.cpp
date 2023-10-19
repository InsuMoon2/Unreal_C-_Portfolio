#include "Weapon/DoActions/CDoAction_AirCombo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CDoAction_Combo.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Weapon/CAttachment.h"

void UCDoAction_AirCombo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);
	
	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction();

	Attachment->OnAttachmentEndCollision.Clear();
	Attachment->OnAttachmentBeginOverlap.Clear();

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCDoAction_AirCombo::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCDoAction_AirCombo::OnAttachmentBeginOverlap);

	GravityVelocityChange(OwnerCharacter, 0.0f, FVector::ZeroVector);

	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_AirCombo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_AirCombo::End_DoAction()
{
	Super::End_DoAction();

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	OwnerCharacter->GetCharacterMovement()->GravityScale = 1.0f;
	State->OffAirActionMode();

	Index = 0;
}

void UCDoAction_AirCombo::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}

void UCDoAction_AirCombo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);

	CheckNull(InOther);

	// Hitted Clear
	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	CheckTrue(HitDatas.Num() - 1 < Index);

	// Profile Change
	{
		ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(OwnerCharacter);

		if(!boss)
		{
			UCapsuleComponent* capsule = InOther->GetCapsuleComponent();
			if (!!capsule)
				capsule->SetCollisionProfileName(TEXT("AirHitted"));
			FName name = InOther->GetCapsuleComponent()->GetCollisionProfileName();
		}

		//CLog::Print(name.ToString());
	}

	GravityVelocityChange(InOther, 1.0f);

	ACharacter* boss = Cast<ACEnemy_Boss>(InOther);
	if (!!boss)
	{
		CheckTrue(Boss_AirHitDatas.Num() - 1 < Index);
		Boss_AirHitDatas[Index].SendDamage(InAttacker, InAttackCauser, boss);

		return;
	}

	CheckTrue(HitDatas.Num() - 1 < Index);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
}

void UCDoAction_AirCombo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();


	Hitted.Empty();
}

void UCDoAction_AirCombo::GravityVelocityChange(ACharacter* InCharacter, float InGravity, FVector InVelocity)
{
	InCharacter->GetCharacterMovement()->GravityScale = InGravity;
	InCharacter->GetCharacterMovement()->Velocity = InVelocity;
}


