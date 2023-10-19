#include "Notifies/CAnimNotify_EndAction.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapon/CDoAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(character);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	UCAIBehaviorComponent* aiState = CHelpers::GetComponent<UCAIBehaviorComponent>(MeshComp->GetOwner());

	CheckNull(weapon);
	CheckNull(state);

	if(!!boss)
		CheckNull(aiState);

	switch (ActionType)
	{
	case EActionType::Action:
		CheckNull(weapon->GetDoAction());
		weapon->GetDoAction()->End_DoAction();
		break;

	case EActionType::AirAction:
		CheckNull(weapon->GetAirAction());
		weapon->GetAirAction()->End_DoAction();

		if (!!boss)
			aiState->SetWaitMode();
		break;
	}
	
}
