#include "Character/Enemies/CEnemy_AI_Range.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Label.h"

ACEnemy_AI_Range::ACEnemy_AI_Range()
{
	PrimaryActorTick.bCanEverTick = true;

	TSubclassOf<UCUserWidget_Label> labelClass;
	CHelpers::GetClass<UCUserWidget_Label>(&labelClass, "WidgetBlueprint'/Game/_Game/Widget/WB_Label.WB_Label_C'");
	LabelWidget->SetWidgetClass(labelClass);
	LabelWidget->SetRelativeLocation(FVector(0, 0, 250));
	LabelWidget->SetDrawSize(FVector2D(200, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy_AI_Range::BeginPlay()
{
	Super::BeginPlay();

	Enemy_Status->InitEnemyHUD(LabelWidget);
}

void ACEnemy_AI_Range::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Enemy_Status->UpdateEnemyHUD(LabelWidget);
}

