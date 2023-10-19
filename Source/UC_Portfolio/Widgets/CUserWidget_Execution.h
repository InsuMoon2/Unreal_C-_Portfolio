#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Execution.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_Execution : public UUserWidget
{
	GENERATED_BODY()
		 
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Text")
		void On_VisibleText();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Text")
		void Off_VisibleText();

};
