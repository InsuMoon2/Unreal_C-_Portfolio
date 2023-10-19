#include "Behaviors/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "NavigationSystem.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	// 반드시 켜야 Tick이 실행된다.
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);
	// PatrolPath Exist
	if (!!ai->GetPatrolPath())
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		movement->OnSprint();

		if (bDebugMode)
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 25, 25,
				FColor::Red, true, 5);

		return EBTNodeResult::InProgress;
	}

	FVector location = ai->GetActorLocation();

	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navSystem, EBTNodeResult::Failed);

	FNavLocation point(location);
	while (true)
	{
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point))
			break;
	}

	
	behavior->SetPatrolLocation(point.Location);

	if (bDebugMode)
		DrawDebugSphere(ai->GetWorld(), point.Location, 25, 25,
			FColor::Green, true, 5);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	//controller->K2_ClearFocus();

	FVector location = behavior->GetPatrolLoaction();
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, false);

	switch (result)
	{
	case EPathFollowingRequestResult::Failed:
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		if (ai->GetPatrolPath())
			ai->GetPatrolPath()->UpdateIndex();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	break;

	}
}
