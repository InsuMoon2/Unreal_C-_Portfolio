#include "Components/CInGameUIComponent.h"
#include "Global.h"
#include "CStatusComponent.h"
#include "CWeaponComponent.h"
#include "Character/CPlayer.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/CAttachment.h"
#include "Widgets/CUserWidget_HUD.h"
#include "Widgets/CUserWidget_Label.h"
#include "Widgets/CUserWidget_Boss.h"
#include "Widgets/CUserWidget_SkillLack.h"

UCInGameUIComponent::UCInGameUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCInGameUIComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);

}

void UCInGameUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UCInGameUIComponent::InitPlayerHUD()
{
	if (!!Player_HUD)
	{
		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		CheckNull(player);

		player_hud = CreateWidget<UCUserWidget_HUD>(OwnerCharacter->GetWorld(), Player_HUD);
		player_hud->AddToViewport();
		player_hud->SetVisibility(ESlateVisibility::Visible);

		player_hud->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		player_hud->UpdateMana(Status->GetMana(), Status->GetMaxMana());
	}
}

void UCInGameUIComponent::InitEnemyHUD(class UWidgetComponent* InWidgetComponent)
{
	if (!!Enemy_HUD)
	{
		ACEnemy_AI* enemy = Cast<ACEnemy_AI>(OwnerCharacter);
		CheckNull(enemy);

		enemy_hud = Cast<UCUserWidget_Label>(InWidgetComponent->GetUserWidgetObject());
		enemy_hud->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		enemy_hud->UpdateIncapacitation(Status->GetIncapacitation(), Status->GetMaxIncapacitation());

		enemy_hud->UpdateName(OwnerCharacter->GetName());
		enemy_hud->UpdateControllerName(OwnerCharacter->GetController()->GetName());
	}
}

void UCInGameUIComponent::UpdatePlayerHUD()
{
	if (!!Player_HUD)
	{
		player_hud->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		player_hud->UpdateMana(Status->GetMana(), Status->GetMaxMana());
	}
}

void UCInGameUIComponent::UpdateEnemyHUD(class UWidgetComponent* InWidgetComponent)
{
	if (!!Enemy_HUD)
	{
		ACEnemy_AI* enemy = Cast<ACEnemy_AI>(OwnerCharacter);
		CheckNull(enemy);

		enemy_hud->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		enemy_hud->UpdateIncapacitation(Status->GetIncapacitation(), Status->GetMaxIncapacitation());

		UpdateLabelRenderScale();
	}
}

void UCInGameUIComponent::InitBossHUD()
{
	if (!!Boss_HUD)
	{
		ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(OwnerCharacter);
		CheckNull(boss);

		boss_hud = CreateWidget<UCUserWidget_Boss>(OwnerCharacter->GetWorld(), Boss_HUD);
		boss_hud->AddToViewport();
		boss_hud->SetVisibility(ESlateVisibility::Visible);

		boss_hud->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		boss_hud->UpdateIncapacitation(Status->GetIncapacitation(), Status->GetMaxIncapacitation());
	}
}

void UCInGameUIComponent::UpdateBossHUD()
{
	if (!!Boss_HUD)
	{
		ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(OwnerCharacter);
		CheckNull(boss);

		boss_hud->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		boss_hud->UpdateIncapacitation(Status->GetIncapacitation(), Status->GetMaxIncapacitation());
	}
}

void UCInGameUIComponent::UpdateLabelRenderScale()
{
	CheckNull(Enemy_HUD);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(OwnerCharacter->GetWorld(), 0);
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(OwnerCharacter->GetWorld(), 0);

	FVector playerLocation = player->GetActorLocation();

	FVector cameraLocation = cameraManager->GetCameraLocation();
	FVector targetLocation = OwnerCharacter->GetController()->GetTargetLocation();

	float distance = FVector::Distance(playerLocation, OwnerCharacter->GetActorLocation());
	float sizeRate = 1.0f - (distance / LabelViewAmount);

	if (distance > LabelViewAmount)
	{
		enemy_hud->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	enemy_hud->SetVisibility(ESlateVisibility::Visible);
	enemy_hud->SetRenderScale(FVector2D(sizeRate, sizeRate));
}

void UCInGameUIComponent::ShowLackText(bool InCheck)
{
	if (!!Lack_HUD)
	{
		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		CheckNull(player);

		LackText = CreateWidget<UCUserWidget_SkillLack>(OwnerCharacter->GetWorld(), Lack_HUD);
		LackText->AddToViewport();

		if (InCheck)
			LackText->VisibleText();

		else if (!InCheck)
			LackText->InVisibleText();
	}
}

void UCInGameUIComponent::ShowCoolTimeText(bool InCheck)
{
	if (!!Player_HUD)
	{
		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		CheckNull(player);

		if (InCheck == true)
			player_hud->VisibleCoolTimeText();

		else if (InCheck == false)
			player_hud->InVisibleCoolTimeText();
	}
}

void UCInGameUIComponent::ShowExecutionText(bool InCheck)
{
	if (!!Player_HUD)
	{
		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		CheckNull(player);

		if (InCheck == true)
			player_hud->VisibleExecutionText();

		else if (InCheck == false)
			player_hud->InVisibleExecutionText();
	}
}
