#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RandomIndex.generated.h"

UCLASS()
class UC_PORTFOLIO_API UBTTaskNode_RandomIndex : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_RandomIndex();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
