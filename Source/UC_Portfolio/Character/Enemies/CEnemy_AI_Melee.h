#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "CEnemy_AI_Melee.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACEnemy_AI_Melee
	: public ACEnemy_AI
{
	GENERATED_BODY()

private:

public:
	ACEnemy_AI_Melee();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
