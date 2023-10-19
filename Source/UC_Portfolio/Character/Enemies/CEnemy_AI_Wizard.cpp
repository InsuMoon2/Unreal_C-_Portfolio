#include "Character/Enemies/CEnemy_AI_Wizard.h"
#include "Global.h"
#include "Components/CInGameUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Label.h"

ACEnemy_AI_Wizard::ACEnemy_AI_Wizard()
{
	PrimaryActorTick.bCanEverTick = true;

	TSubclassOf<UCUserWidget_Label> labelClass;
	CHelpers::GetClass<UCUserWidget_Label>(&labelClass, "WidgetBlueprint'/Game/_Game/Widget/WB_Label.WB_Label_C'");
	LabelWidget->SetWidgetClass(labelClass);
	LabelWidget->SetRelativeLocation(FVector(0, 0, 250));
	LabelWidget->SetDrawSize(FVector2D(200, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy_AI_Wizard::BeginPlay()
{
	Super::BeginPlay();

	Enemy_Status->InitEnemyHUD(LabelWidget);
}

void ACEnemy_AI_Wizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Enemy_Status->UpdateEnemyHUD(LabelWidget);
}