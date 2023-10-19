#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_MoveToTarget.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_MoveToTarget();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Distance")
		float SetDistance;
};

