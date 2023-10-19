#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInGameUIComponent.generated.h"



UCLASS()
class UC_PORTFOLIO_API UCInGameUIComponent : public UActorComponent
{
	GENERATED_BODY()

private:


public:	
	UCInGameUIComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void InitPlayerHUD();
	void InitEnemyHUD(class UWidgetComponent* InWidgetComponent);

	void UpdatePlayerHUD();
	void UpdateEnemyHUD(class UWidgetComponent* InWidgetComponent);

	void InitBossHUD();
	void UpdateBossHUD();

	void UpdateLabelRenderScale();

	void ShowLackText(bool InCheck);
	void ShowCoolTimeText(bool InCheck);
	void ShowExecutionText(bool InCheck);

private:
	// 머리 위에 Widget의 크기를 점점 줄여줄 변수
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float LabelViewAmount = 3000.0f;

private:
	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<class UCUserWidget_HUD> Player_HUD;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<class UCUserWidget_Label> Enemy_HUD;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<class UCUserWidget_Boss> Boss_HUD;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<class UCUserWidget_SkillLack> Lack_HUD;
	
	UPROPERTY(EditAnywhere, Category = "LabelCheck")
		bool bLabelCheck = true;

	class UCUserWidget_Label*	  enemy_hud;
	class UCUserWidget_HUD*		  player_hud;
	class UCUserWidget_Boss*	  boss_hud;
	class UCUserWidget_SkillLack* LackText;

	class ACharacter* OwnerCharacter;
	class UCStatusComponent* Status;
	class UCWeaponComponent* Weapon;
};
