#include "Behaviors/CBTService_Wizard.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Character/Enemies/CAIController.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CEquipment.h"
#include "Weapon/SubActions/CSubAction_Bow.h"

UCBTService_Wizard::UCBTService_Wizard()
{
	NodeName = "Wizard";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Wizard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNull(weapon);

	SkillTime = UKismetMathLibrary::FClamp(aiState->GetSkillTime() + DeltaSeconds, 0.0f, 5.0f);
	aiState->SetSkillTime(SkillTime);

	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();
		start.Z -= 25;

		FVector end = start;
		end.Z += 50;

		DrawDebugCylinder(ai->GetWorld(), start, end, AvoidRange, 5,
			FColor::Red, true, Interval);
	}

	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();

		return;
	}

	ACharacter* target = aiState->GetTarget();
	if (target == nullptr)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		aiState->SetPatrolMode();
		//aiState->SetWaitMode();

		return;
	}

	controller->SetFocus(target);
	float distance = ai->GetDistanceTo(target);

	if (distance < ActionRange)
	{
		if (distance < AvoidRange)
		{
			aiState->SetAvoidMode();
		
			return;
		}

		aiState->SetActionMode();

		return;
	}

	aiState->SetApproachMode();

}
