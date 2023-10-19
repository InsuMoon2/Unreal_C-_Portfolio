#include "Weapon/AddOns/CSkill_Rolling.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"

UCSkill_Rolling::UCSkill_Rolling()
{

}

void UCSkill_Rolling::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	PlayerController = Owner->GetController<APlayerController>();
	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);
}

void UCSkill_Rolling::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Pressed(InSkillIndex);

	State->SetRollMode();
	State->OnSkillMode();
	bInAction = true;

	CheckNull(Behavior);
	//Owner->SetActorLocation(Behavior->GetAvoidLocation());
	FVector direction = Behavior->GetAvoidLocation() - Owner->GetActorLocation();
	direction = direction.GetSafeNormal2D();

	FRotator rotator = FRotator(
		0, direction.Rotation().Yaw, 0);

	Owner->SetActorRotation(rotator);

	SkillData.DoAction(Owner);
}

void UCSkill_Rolling::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(InSkillIndex);
}

void UCSkill_Rolling::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
}

void UCSkill_Rolling::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	
}

void UCSkill_Rolling::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();

	bInAction = false;
	Behavior->SetWaitMode();
	Behavior->SetSkillTime(0.0f);

	Movement->Move();
}
