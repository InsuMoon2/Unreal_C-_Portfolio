#include "Notifies/CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapon/CDoAction.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin_DoAction";

}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(state);

	switch (ActionType)
	{
	case EActionType::Action:
		CheckNull(weapon->GetDoAction());
		weapon->GetDoAction()->Begin_DoAction();
		break;

	case EActionType::AirAction: 
		CheckNull(weapon->GetAirAction());
		weapon->GetAirAction()->Begin_DoAction();
		break;
	}

}
