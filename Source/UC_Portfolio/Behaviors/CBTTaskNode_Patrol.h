#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Patrol")
        bool bDebugMode;

    UPROPERTY(EditAnywhere, Category = "Patrol")
        float AcceptanceDistance = 65;

    UPROPERTY(EditAnywhere, Category = "Random")
        float RandomRadius = 1500;

public:
    UCBTTaskNode_Patrol();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
