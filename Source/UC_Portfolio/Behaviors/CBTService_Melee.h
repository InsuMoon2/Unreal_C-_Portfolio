#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 170;

	UPROPERTY(EditAnywhere, Category = "Approach")
		float ApproachRange = 300;

	UPROPERTY(EditAnywhere, Category = "SkillTime")
		float SkillTime = 0.0f;

public:
	UCBTService_Melee();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
