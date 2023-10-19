#include "Behaviors/CBTTaskNode_Speed.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CEnemy.h"
#include "Character/Enemies/CAIController.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);

	movement->SetSpeed(Type);

	return EBTNodeResult::Succeeded;
}
