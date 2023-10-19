#include "Behaviors/BTTaskNode_RandomIndex.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CSkill.h"

UBTTaskNode_RandomIndex::UBTTaskNode_RandomIndex()
{
	NodeName = "Random";

	bNotifyTick = false;
}

EBTNodeResult::Type UBTTaskNode_RandomIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	int32 random = UKismetMathLibrary::RandomIntegerInRange(
		(uint8)EAISkillType::Skill1, (uint8)EAISkillType::Max - 1);

	aiState->SetSkill_Type((EAISkillType)random);
	//CLog::Print(random);

	return EBTNodeResult::Succeeded;
}
