#include "Behaviors/CBTService_Range.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Character/Enemies/CAIController.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/SubActions/CSubAction_Bow.h"

UCBTService_Range::UCBTService_Range()
{
	NodeName = "Range";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Range::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	// 스킬 쿨타임
	SkillTime = UKismetMathLibrary::FClamp(aiState->GetSkillTime() + DeltaSeconds, 0.0f, 5.0f);
	aiState->SetSkillTime(SkillTime);

	//CLog::Print(SkillTime);

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

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNull(weapon);

	UCSubAction_Bow* bow = nullptr;
	bow = Cast<UCSubAction_Bow>(weapon->GetSubAction());

	ACharacter* target = aiState->GetTarget();
	if (target == nullptr)
	{
		if (!!bow)
			bow->Released();

		controller->ClearFocus(EAIFocusPriority::Gameplay);
		aiState->SetPatrolMode();
		//aiState->SetWaitMode();

		return;
	}

	controller->SetFocus(target);
	float distance = ai->GetDistanceTo(target);

	if (aiState->IsAvoidMode())
		controller->ClearFocus(EAIFocusPriority::Gameplay);

	if (distance < ActionRange)
	{
		if (distance < AvoidRange && SkillTime >= 5.0f)
		{
			// 공격 도중이 아닐 때에만 상태 변경
			if (!state->IsActionMode())
			{
				if (!!bow)
					bow->Released();

				controller->ClearFocus(EAIFocusPriority::Gameplay);
				aiState->SetAvoidMode();

				return;
			}
		}

		if (!!bow)
			bow->Pressed();

		if (state->IsIdleMode())
			aiState->SetActionMode();

		return;
	}

	if (!!bow)
		bow->Released();

	aiState->SetApproachMode();
		
}
