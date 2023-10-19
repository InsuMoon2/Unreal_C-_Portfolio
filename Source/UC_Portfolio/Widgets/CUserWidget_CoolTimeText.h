#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CoolTimeText.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_CoolTimeText : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CoolTimeText")
		void VisibleText();
		
	
};
