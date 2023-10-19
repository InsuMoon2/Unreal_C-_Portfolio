#include "Weapon/AddOns/CSkill_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"

UCSkill_Warp::UCSkill_Warp()
{

}

void UCSkill_Warp::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	PlayerController = Owner->GetController<APlayerController>();
	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(InOwner);
}

void UCSkill_Warp::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Pressed(InSkillIndex);

	State->SetActionMode();
	State->OnSkillMode();
	bInAction = true;

	SkillData.DoAction(Owner);
}

void UCSkill_Warp::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(InSkillIndex);
}

void UCSkill_Warp::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

}

void UCSkill_Warp::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	CheckNull(Behavior);
	Owner->SetActorLocation(Behavior->GetAvoidLocation(), false, nullptr, ETeleportType::TeleportPhysics);
}

void UCSkill_Warp::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();

	bInAction = false;

	Movement->Move();

}
