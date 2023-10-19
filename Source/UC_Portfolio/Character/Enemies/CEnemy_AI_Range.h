#pragma once

#include "CoreMinimal.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "CEnemy_AI_Range.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACEnemy_AI_Range : public ACEnemy_AI
{
	GENERATED_BODY()

private:

public:
	ACEnemy_AI_Range();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
};
