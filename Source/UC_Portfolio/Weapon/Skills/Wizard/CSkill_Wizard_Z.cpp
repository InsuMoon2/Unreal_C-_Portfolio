#include "Weapon/Skills/Wizard/CSkill_Wizard_Z.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"

UCSkill_Wizard_Z::UCSkill_Wizard_Z()
{
	bShowCursor = false;
}

void UCSkill_Wizard_Z::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	Decal = CHelpers::GetComponent<UDecalComponent>(InAttachment);
	PlayerController = Owner->GetController<APlayerController>();
}

void UCSkill_Wizard_Z::Skill_Pressed(ESkillType InSkillIndex)
{
	Super::Skill_Pressed(ESkillType::Skill1);

	// On Off
	//bShowCursor = !bShowCursor;

	if (bShowCursor)
	{
		bInAction = true;

		FRotator rotation;
		if (GetCursorLocationAndRotation(MoveToLocation, rotation))
		{
			float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			MoveToLocation = FVector(MoveToLocation.X, MoveToLocation.Y, MoveToLocation.Z + height);

			float yaw = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), MoveToLocation).Yaw;
			Owner->SetActorRotation(FRotator(0, yaw, 0));
		}

		else
			return;

		SkillData.DoAction(Owner);
	}

	else 
		bShowCursor = true;

}

void UCSkill_Wizard_Z::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill1);


}

void UCSkill_Wizard_Z::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;
	
	if (GetCursorLocationAndRotation(location, rotation) == false)
	{
		Decal->SetVisibility(false);

		return;
	}

	if (bInAction)
		return;

	if (bShowCursor)
	{
		Decal->SetVisibility(true);
		Decal->SetWorldLocation(location);
		Decal->SetWorldRotation(rotation);
	}
		

	else
		Decal->SetVisibility(false);

	//CLog::Print(bShowCursor);

}

void UCSkill_Wizard_Z::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	CheckFalse(State->IsIdleMode());

	Owner->SetActorLocation(MoveToLocation);
	MoveToLocation = FVector::ZeroVector;

	//bShowCursor = false;
}

void UCSkill_Wizard_Z::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	bShowCursor = false;
	bInAction = false;
	Movement->Move();
	Camera->DisableFixedCamera();
}

bool UCSkill_Wizard_Z::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	CheckNullResult(PlayerController, false);

	FHitResult hitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);
	CheckFalseResult(hitResult.bBlockingHit, false);

	OutLocation = hitResult.Location;
	OutRotation = hitResult.ImpactNormal.Rotation();

	return true;
}
