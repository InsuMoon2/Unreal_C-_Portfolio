#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Boss.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_Boss : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Status")
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Status")
		void UpdateIncapacitation(float InIncapacitation, float InMaxIncapacitation);

};
