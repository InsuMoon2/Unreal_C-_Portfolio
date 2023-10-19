#include "Behaviors/CBTService_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Character/Enemies/CAIController.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Boss::UCBTService_Boss()
{
	NodeName = "Boss";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	SkillTime = UKismetMathLibrary::FClamp(aiState->GetSkillTime() + DeltaSeconds, 0.0f, 8.0f);
	aiState->SetSkillTime(SkillTime);

	if (state->IsHittedMode() || state->IsIncapacitationMode())
	{
		aiState->SetHittedMode();
		controller->K2_ClearFocus();

		return;
	}

	ACharacter* target = aiState->GetTarget();
	if (target == nullptr)
	{
		controller->K2_ClearFocus();
		aiState->SetWaitMode();

		return;
	}

	if (!state->IsActionMode())
		controller->SetFocus(target);

	else
		controller->K2_ClearFocus();

	aiState->SetDistance(ai->GetDistanceTo(target));
	float distance = aiState->GetDistance();

	if (distance < ActionRange)
	{
		aiState->SetActionMode();

		return;
	}

	if (aiState->IsApproachMode() && state->IsActionMode())
		controller->StopMovement();

	//if (!state->IsActionMode())
	//	aiState->SetApproachMode();
	
}
