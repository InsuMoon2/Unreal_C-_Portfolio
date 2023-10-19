#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h"
#include "CUserWidget_Interface_Skill.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_Interface_Skill : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "WeaponType")
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WeaponType")
		EWeaponType WeaponType = EWeaponType::UnArmed;

private:
	class UCWeaponComponent* WeaponComponent;
};
