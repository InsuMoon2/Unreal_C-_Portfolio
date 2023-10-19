#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController_Boss.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACAIController_Boss : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;

public:
	ACAIController_Boss();

protected:
	void BeginPlay() override;

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	class ACEnemy_Boss* Boss;
	class UCAIBehaviorComponent* Behavior;

	class UAISenseConfig_Sight* Sight;

};
