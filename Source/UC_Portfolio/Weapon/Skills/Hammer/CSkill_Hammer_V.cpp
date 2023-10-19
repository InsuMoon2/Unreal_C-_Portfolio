#include "Weapon/Skills/Hammer/CSkill_Hammer_V.h"

#include "CSkill_Hammer_V_Explosion.h"
#include "CSkill_Hammer_V_Ground.h"
#include "CSkill_Hammer_V_Shield.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCSkill_Hammer_V::Skill_Pressed(ESkillType InSkillIndex)
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

	// Sheld Spawn
	FVector Location = FVector(Owner->GetActorLocation());

	FTransform transform;
	transform.SetLocation(FVector(Location.X, Location.Y, Location.Z));
	transform.SetScale3D(Shield_Scale);

	Shield = Owner->GetWorld()->SpawnActor<ACSkill_Hammer_V_Shield>
		(Spawn_Shield_Class, transform);

	//CHelpers::AttachTo(Shield, Owner->GetMesh(), TEXT("Root"));
	Shield->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("root"));
	Shield->ShieldSmall(Owner);

	// Ground Spawn
	FTransform transform2;
	transform2.SetLocation(FVector(Location.X, Location.Y, Location.Z - 70.0f));
	transform2.SetScale3D(Ground_Scale);

	Ground = Owner->GetWorld()->SpawnActor<ACSkill_Hammer_V_Ground>
		(Spawn_Ground_Class, transform2);

	//CHelpers::AttachTo(Ground, Owner->GetMesh(), TEXT("Root"));
	Ground->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("root"));
	Ground->GroundSmall(Owner);

	SkillData.DoAction(Owner);
}

void UCSkill_Hammer_V::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(ESkillType::Skill4);
}

void UCSkill_Hammer_V::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Skill_CoolTime = Weapon->SetCoolTime(Skill_CoolTime, Skill_MaxCoolTime, InDeltaTime);
}

void UCSkill_Hammer_V::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

}

void UCSkill_Hammer_V::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();
	Camera->DisableFixedCamera();
}
