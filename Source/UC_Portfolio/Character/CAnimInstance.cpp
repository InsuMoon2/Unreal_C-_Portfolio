#include "Character/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/CSubAction.h"
#include "Weapon/SubActions/CSubACtion_Bow.h"
#include "Parkour/CParkourComponent.h"
#include "Components/CFeetComponent.h"
#include "Components/CMovementComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	if (!!Weapon)
		Weapon->OnWeaponTypeChange.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();
	Falling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);

	// Bow
	{
		CheckNull(Weapon);
		if (!!Weapon->GetSubAction())
		{
			bBow_Aiming = true;
			bBow_Aiming &= WeaponType == EWeaponType::Bow;
			bBow_Aiming &= Weapon->GetSubAction()->GetInAction();
		}
	}

	// IK
	{
		//CheckFalse(Weapon->IsUnarmedMode());
		bFeet = false;

		if (!!parkour && !!feet)
		{
			bFeet = parkour->IsExecuting() == false;
			FeetData = feet->GetData();
		}
		else if (!!feet)
		{
			bFeet = true;
			FeetData = feet->GetData();
		}
	}
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
