#include "Behaviors/CBTService_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Character/Enemies/CAIController.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStatusComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	SkillTime = UKismetMathLibrary::FClamp(aiState->GetSkillTime() + DeltaSeconds, 0.0f, 5.0f);
	aiState->SetSkillTime(SkillTime);

	if (status->GetHealth() <= 0.0f)
		controller->ClearFocus(EAIFocusPriority::Gameplay);

	if (state->IsHittedMode() || state->IsIncapacitationMode())
	{
		aiState->SetHittedMode();
		controller->ClearFocus(EAIFocusPriority::Gameplay);

		return;
	}

	ACharacter* target = aiState->GetTarget();
	if (target == nullptr)
	{
		aiState->SetPatrolMode();

		return;
	}

	//controller->ClearFocus(EAIFocusPriority::Gameplay);

	if (!state->IsActionMode())
		controller->SetFocus(target);

	/*else
		controller->ClearFocus(EAIFocusPriority::Gameplay);*/

	float distance = ai->GetDistanceTo(target);
	
	if (distance < ActionRange)
	{
		aiState->SetActionMode();
		//controller->SetFocus(target);

		return;
	}

	aiState->SetApproachMode();

	if (aiState->IsApproachMode())
	{
		if (state->IsActionMode())
			controller->StopMovement();
	}

}
