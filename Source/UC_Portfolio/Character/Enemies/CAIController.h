#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;

public:
	ACAIController();

protected:
	void BeginPlay() override;

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	class ACEnemy_AI* Enemy;
	class ACCharacter* Character;
	class UCAIBehaviorComponent* Behavior;

	// 이 안에 감지객체가 다 몰려있다.
	class UAISenseConfig_Sight* Sight;
	
};
