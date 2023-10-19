#include "Behaviors/CBTTaskNode_MoveToTarget.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"

UCBTTaskNode_MoveToTarget::UCBTTaskNode_MoveToTarget()
{
	NodeName = "MoveToTarget";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	controller->MoveToActor(aiState->GetTarget());

	controller->SetFocus(aiState->GetTarget());

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	float distance = aiState->GetDistance();

	bool bCheck = true;
	bCheck &= (distance < SetDistance);

	if (bCheck)
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}

	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}
}
