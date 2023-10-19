#include "Character/Enemies/CEnemy_AI.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/TextRenderComponent.h"
#include "Widgets/CUserWidget_Label.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UTextRenderComponent>(this, &AIState, "AIState", GetMesh());
	CHelpers::CreateComponent<UTextRenderComponent>(this, &TeamID_Text, "TeamID", GetMesh());
	CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent<UCInGameUIComponent>(this, &Enemy_Status, "Enemy_Status");

	AIState->SetRelativeLocation(FVector(0, 0, 220));
	AIState->SetRelativeRotation(FRotator(0, 90, 0));
	AIState->SetHorizontalAlignment(EHTA_Center);
	AIState->SetVerticalAlignment(EVRTA_TextCenter);
	AIState->SetTextRenderColor(FColor::Red);

	TeamID_Text->SetRelativeLocation(FVector(0, 0, 300));
	TeamID_Text->SetRelativeRotation(FRotator(0, 90, 0));
	TeamID_Text->SetHorizontalAlignment(EHTA_Center);
	TeamID_Text->SetVerticalAlignment(EVRTA_TextCenter);
	TeamID_Text->SetTextRenderColor(FColor::Blue);

	CHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());

	//TeamID = 2;

	TeamID_Text->SetVisibility(false);
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString str = UEnum::GetValueAsString<EAIStateType>(Behavior->GetAIStateType());
	AIState->SetText(UKismetTextLibrary::Conv_StringToText(str));
	TeamID_Text->SetText(FString::FromInt(GetTeamID()));
	
	//CLog::Print(Status->GetIncapacitation());


}

void ACEnemy_AI::Hitted()
{
	// 죽으면 처리하지 않도록
	CheckTrue(State->IsDeadMode());

	Super::Hitted();

	Behavior->SetHittedMode();
}

void ACEnemy_AI::End_Hitted()
{
	Super::End_Hitted();

	Behavior->SetWaitMode();
}

void ACEnemy_AI::End_Incapacitation()
{
	Super::End_Incapacitation();

	Behavior->SetWaitMode();
}
