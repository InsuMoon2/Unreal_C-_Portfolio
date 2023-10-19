#include "Behaviors/CBTTaskNode_Skill.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CSkill.h"	
#include "Weapon/Skills/CSkill_Upper.h"

UCBTTaskNode_Skill::UCBTTaskNode_Skill()
{
	NodeName = "Skill";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(ai);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	// 스킬을 사용할 때마다 스킬 시간 초기화
	aiState->SetSkillTime(0.0f);

	controller->StopMovement();

	switch (Type)
	{
	case ESkillType::Skill1:
		if (!!boss)
		{
			weapon->Skill_AirAction();
			break;
		}

		weapon->Skill1_Pressed();
		break;

	case ESkillType::Skill2:
		weapon->Skill2_Pressed();
		break;

	case ESkillType::Skill3:
		weapon->Skill3_Pressed();
		break;

	case ESkillType::Skill4:
		weapon->Skill4_Pressed();
		break;

	case ESkillType::Skill_Upper:
		weapon->Skill_AirAction();
		break;

	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Skill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(weapon);
	CheckNull(state);

	bool bCheck = true;
	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetSkill((int32)Type)->GetInAction() == false);

	UCSkill_Upper* upper = Cast<UCSkill_Upper>(weapon->GetSkill((int32)Type));
	if (!!upper)	// 맞았으면
	{
		bCheck &= upper->IsExistHitted();
		//bCheck &= upper->GetHitted();
	}

	//CLog::Print(weapon);

	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		if (!!upper)
			upper->SetHittedEmpty();

		return;
	}

	//else if (bCheck && !upper->GetHitted())
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
	//
	//	return;
	//}

}

EBTNodeResult::Type UCBTTaskNode_Skill::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	if (ai == nullptr)
	{
		//CLog::Print(ai);
		return EBTNodeResult::Failed;
	}

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	if (weapon == nullptr)
		return EBTNodeResult::Failed;

	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	int32 random = UKismetMathLibrary::RandomIntegerInRange(
		(uint8)EAISkillType::Skill1, (uint8)EAISkillType::Max - 1);

	aiState->SetSkill_Type((EAISkillType)random);
	//CLog::Print("Skill Abort?");

	//if (!weapon->IsBowMode() && aiState->IsApproachMode())
	//{
	//	bool bBeginAction = weapon->GetSkill((int32)WeaponType)->GetInAction();
	//	if (bBeginAction == false)
	//	{
	//		weapon->GetSkill((int32)WeaponType)->Skill_Pressed(WeaponType);
	//	}
	//}

	weapon->GetSkill((int32)Type)->End_Skill();

	return EBTNodeResult::Succeeded;
}
