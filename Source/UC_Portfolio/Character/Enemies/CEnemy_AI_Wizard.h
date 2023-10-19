#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "CEnemy_AI_Wizard.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACEnemy_AI_Wizard : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	ACEnemy_AI_Wizard();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
