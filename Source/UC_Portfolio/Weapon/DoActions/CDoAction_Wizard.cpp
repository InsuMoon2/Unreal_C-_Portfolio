#include "Weapon/DoActions/CDoAction_Wizard.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/AddOns/CFireBall.h"

UCDoAction_Wizard::UCDoAction_Wizard()
{

}

void UCDoAction_Wizard::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	const TArray<FDoActionData>& InDoActionData, const TArray<FHitData>& InHitData, const TArray<FHitData>& InGuardData, const TArray<FHitData>& InBossHitDatas, const TArray<FHitData>& InBossAirHitDatas)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionData, InHitData, InGuardData, InBossHitDatas, InBossAirHitDatas);

	
}

void UCDoAction_Wizard::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	
}

void UCDoAction_Wizard::DoAction()
{
	CheckFalse(State->IsIdleMode());

	Super::DoAction();

	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Wizard::Begin_DoAction()
{
	Super::Begin_DoAction();

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(FName("Hand_Wizard"));
	FVector forward = OwnerCharacter->GetActorForwardVector();

	FTransform transform;
	transform.SetLocation(location);
	transform.SetRotation(FQuat(forward.Rotation()));
	transform.SetScale3D(FVector(1));

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;
	ACFireBall* fire = Cast<ACFireBall>(OwnerCharacter->GetWorld()->SpawnActor(fireball, &transform, params));
	fire->OnOverlap.AddDynamic(this, &UCDoAction_Wizard::OnFireBallOverlap);

	fire->Shoot(forward);
}

void UCDoAction_Wizard::End_DoAction()
{
	Super::End_DoAction();

}

void UCDoAction_Wizard::OnFireBallOverlap(AActor* InCauser, ACharacter* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);
	int32 guard_hitted = UKismetMathLibrary::RandomIntegerInRange(0, GuardHitDatas.Num() - 1);

	CheckTrue(CHelpers::IsSameTeam(OwnerCharacter, InOtherCharacter));

	// 원거리 공격은 패링 안되게
	{
		int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(
			0, GuardHitDatas.Num() - 1);

		// 맞는 놈의 State
		UCStateComponent* hitted_state = CHelpers::GetComponent<UCStateComponent>(InOtherCharacter);
		UCWeaponComponent* hitted_weapon = CHelpers::GetComponent<UCWeaponComponent>(InOtherCharacter);
		CheckNull(hitted_state);
		CheckNull(hitted_weapon);

		if (hitted_state->IsGuardMode())
		{
			CheckTrue(GuardHitDatas.Num() - 1 < 0);

			bool bGuardCheck = false;
			bGuardCheck = CHelpers::DotForward(InCauser, InOtherCharacter);

			// 가드 성공
			if (bGuardCheck)
			{
				GuardHitDatas[guard_hitted].PlayCameraShake(OwnerCharacter->GetWorld());
				GuardHitDatas[guard_hitted].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);

				hitted_state->SetGuardMode();
			}
			// 가드 실패
			else
			{
				CheckTrue(HitDatas.Num() - 1 < 0);
				HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
			}
		}
		// 그냥 맞을 때
		else
		{
			CheckTrue(HitDatas.Num() - 1 < 0);
			HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
		}
	}

	InCauser->Destroy();
}


