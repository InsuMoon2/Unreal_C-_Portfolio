#include "Behaviors/CBTTaskNode_Equip.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Weapon/CEquipment.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	// 무기가 같다면 성공인 상황일 것이다.
	if (Type == weapon->GetWeaponType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
	case EWeaponType::Bow: weapon->SetBowMode(); controller->StopMovement(); break;
	case EWeaponType::Sword: weapon->SetSwordMode(); break;
	case EWeaponType::Hammer: weapon->SetHammerMode(); break;
	case EWeaponType::Wizard: weapon->SetWizardMode(); break;
	case EWeaponType::GruxWeapon: weapon->SetGruxWeaponMode(); break;

	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNull(weapon);

	const bool* bEquipped = weapon->GetEquipment()->GetEquipped();

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	if (*bEquipped)
	{
		if (state->IsIdleMode())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			return;
		}
	}
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	//CLog::Print("Equip AbortTask");
	if (weapon == nullptr)
		return EBTNodeResult::Failed;

	bool bBeginEquip = weapon->GetEquipment()->GetBeginEquip();
	if (bBeginEquip == false)
		weapon->GetEquipment()->Begin_Equip();

	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Succeeded;
}
