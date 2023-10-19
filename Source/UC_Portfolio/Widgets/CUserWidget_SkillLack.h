#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SkillLack.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_SkillLack : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillLack")
		void VisibleText();

	UFUNCTION(BlueprintImplementableEvent, Category = "SkillLack")
		void InVisibleText();

};
