#include "Behaviors/CBTTaskNode_AirAction.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CDoAction.h"	
#include "Weapon/DoActions/CDoAction_AirCombo.h"

UCBTTaskNode_AirAction::UCBTTaskNode_AirAction()
{
	NodeName = "Air_Action";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_AirAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

void UCBTTaskNode_AirAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCDoAction_AirCombo* doaction_AirCombo = Cast<UCDoAction_AirCombo>(weapon->GetAirAction());

	// ÄÞº¸ °ø°Ý
	if (!!doaction_AirCombo)
	{
		doaction_AirCombo->SetExistCombo(true);
		//CLog::Print(doaction_AirCombo->GetInAction());

	}


	bool bCheck = true;
	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetDoAction()->GetInAction() == false);

	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_AirAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (weapon == nullptr)
		return EBTNodeResult::Failed;

	//if (!state->IsActionMode())
	//{
	//	bool bBeginAction = weapon->GetAirAction()->GetBeginAction();
	//	if (bBeginAction)
	//	{
	//		weapon->GetAirAction()->Begin_DoAction();
	//	}
	//
	//	CLog::Print("Air Action Abort");
	//	weapon->GetAirAction()->End_DoAction();
	//}

	return EBTNodeResult::Aborted;
}
