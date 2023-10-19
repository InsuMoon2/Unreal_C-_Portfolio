#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Wizard.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTService_Wizard : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Action")
		float AvoidRange = 300;

	UPROPERTY(EditAnywhere, Category = "Action")
		bool bDrawDebug;

	UPROPERTY(EditAnywhere, Category = "SkillTime")
		float SkillTime = 0.0f;

public:
	UCBTService_Wizard();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
