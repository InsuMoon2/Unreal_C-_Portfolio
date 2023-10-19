#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h"
#include "CUserWidget_HUD.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "SkillLack")
		void VisibleCoolTimeText();

	UFUNCTION(BlueprintImplementableEvent, Category = "SkillLack")
		void InVisibleCoolTimeText();

	UFUNCTION(BlueprintImplementableEvent, Category = "Execution")
		void VisibleExecutionText();

	UFUNCTION(BlueprintImplementableEvent, Category = "Execution")
		void InVisibleExecutionText();

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Mana")
		void UpdateMana(float InMana, float InMaxMana);

};
