#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class UC_PORTFOLIO_API IICharacter
{
	GENERATED_BODY()

public:

public:
	virtual void End_Roll() {};
	virtual void End_BackStep() {};
	virtual void End_Hitted() {};
	virtual void End_Dead() {};
	virtual void End_Incapacitation() {};
	virtual void End_Guard() {};

public:
	void Create_DynamicMaterial(class ACharacter* InCharacter);
	void Change_Color(class ACharacter* InCharacter, FLinearColor InColor);
};
