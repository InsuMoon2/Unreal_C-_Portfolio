#pragma once

#include "CoreMinimal.h"
#include "CEnemy_AI.h"
#include "CEnemy_Boss.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACEnemy_Boss : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	ACEnemy_Boss();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	void Hitted() override;

public:
	void End_Hitted() override;
	void End_Incapacitation() override;

	
};
