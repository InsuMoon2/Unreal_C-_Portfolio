#include "Behaviors/CBTTaskNode_Action.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CDoAction.h"	
#include "Weapon/DoActions/CDoAction_AirCombo.h"
#include "Weapon/DoActions/CDoAction_Combo.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	//ACharacter* character = aiState->GetTarget();
	//controller->SetFocus(character);

	controller->StopMovement();
	weapon->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCDoAction_Combo* doaction_Combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	UCDoAction_AirCombo* doaction_AirCombo = Cast<UCDoAction_AirCombo>(weapon->GetAirAction());

	// ÄÞº¸ °ø°Ý
	if (!!doaction_Combo)
		doaction_Combo->SetExistCombo(true);

	if (!!doaction_AirCombo)
		doaction_AirCombo->SetExistCombo(true);

	bool bCheck = true;
	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetDoAction()->GetInAction() == false);

	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (weapon == nullptr)
		return EBTNodeResult::Failed;

	if (!state->IsActionMode())
	{
		if (weapon->IsBowMode())
		{
			bool bBeginAction = weapon->GetDoAction()->GetBeginAction();
			if (bBeginAction)
			{
				weapon->GetDoAction()->Begin_DoAction();
			}
		}

		int32 random = UKismetMathLibrary::RandomIntegerInRange(
			(uint8)EAISkillType::Skill1, (uint8)EAISkillType::Max - 1);

		aiState->SetSkill_Type((EAISkillType)random);

		//CLog::Print("Action Abort");
		weapon->GetDoAction()->End_DoAction();
	}

	return EBTNodeResult::Succeeded;
}
