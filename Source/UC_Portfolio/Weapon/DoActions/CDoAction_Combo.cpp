#include "Weapon/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Components/CMontagesComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CAttachment.h"

void UCDoAction_Combo::DoAction()
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

	if (!Attachment->OnAttachmentEndCollision.IsBound())
	{
		Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCDoAction_Combo::OnAttachmentEndCollision);
		Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCDoAction_Combo::OnAttachmentBeginOverlap);
	}

	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}

void UCDoAction_Combo::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	if (!!OwnerController)
		Change_Focus(OwnerController, target);

	//CLog::Print(Index);
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	// Hitted Clear
	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	// 맞는 놈의 State
	UCStateComponent* hitted_state = CHelpers::GetComponent<UCStateComponent>(InOther);
	UCWeaponComponent* hitted_weapon = CHelpers::GetComponent<UCWeaponComponent>(InOther);
	UCMovementComponent* hitted_movement = CHelpers::GetComponent<UCMovementComponent>(InOther);
	CheckNull(hitted_state);
	CheckNull(hitted_weapon);

	CheckTrue(CHelpers::IsSameTeam(OwnerCharacter,InOther));

	// Change Character Rotation
	{
		ACharacter* candidate = nullptr;
		float angle = 0.00001f;

		candidate = CHelpers::GetNearlyFrontAngle(OwnerCharacter, Hitted, angle);

		if (!!candidate)
		{
			OwnerController = OwnerCharacter->GetController();

			rotator = UKismetMathLibrary::FindLookAtRotation(
				OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());

			target = FRotator(0.0f, rotator.Yaw, 0.0f);
			//OwnerController->SetControlRotation(target);
		}
	}

	// Parrying
	{
		int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(0, GuardHitDatas.Num() - 1);

		if (hitted_state->IsGuardMode())
		{
			CheckTrue(GuardHitDatas.Num() - 1 < Index);
			
			bool bGuardCheck = false;
			bGuardCheck = CHelpers::DotForward(InAttacker, InOther);

			// 가드 성공
			if (bGuardCheck)
				//패링
			{	
				if (hitted_state->IsParryingMode())
				{
					hitted_weapon->Skill_Parrying();
					Montage->PlayParryingHitted();
					State->SetHittedMode();
				}
				
				else
				{
					GuardHitDatas[RandomInt].PlayCameraShake(OwnerCharacter->GetWorld());
					GuardHitDatas[RandomInt].SendDamage(InAttacker, InAttackCauser, InOther);
				}

				hitted_state->SetGuardMode();
			}
			// 가드 실패
			else
			{
				// Boss Hit는 다른 Animation이 실행되어야 할듯? 리타겟팅이 제대로 안됨.
				ACharacter* Boss = Cast<ACEnemy_Boss>(InOther);
				if (!!Boss)
				{
					CheckTrue(BossHitDatas.Num() - 1 < Index);
					BossHitDatas[Index].SendDamage(InAttacker, InAttackCauser, Boss);

					return;
				}

				CheckTrue(HitDatas.Num() - 1 < Index);
				HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
			}
		}
		// 그냥 맞을 때
		else
		{
			ACEnemy_Boss* Boss = Cast<ACEnemy_Boss>(InOther);
			if (!!Boss)
				CheckTrueResult(BossHitDatas.Num() - 1 > 0, 
					BossHitDatas[Index].SendDamage(InAttacker, InAttackCauser, Boss));

			CheckTrueResult(HitDatas.Num() - 1 > 0, 
				HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther));
		}
	}

}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	Hitted.Empty();
}

void UCDoAction_Combo::Change_Focus(AController* Incontroller, FRotator InTarget)
{
	// 맞은 놈이 있으면
	if (Hitted.Num() > 0)
	{
		// 보간 처리
		FRotator currentRot = Incontroller->GetControlRotation();
		float deltaTime = OwnerCharacter->GetWorld()->GetDeltaSeconds();
		float rotationSpeed = 11.0f;

		// Yaw값만 보간
		FRotator newRot = FMath::RInterpTo(currentRot, InTarget, deltaTime, rotationSpeed);
		OwnerController->SetControlRotation(FRotator(currentRot.Pitch, newRot.Yaw, currentRot.Roll));
	}
}







