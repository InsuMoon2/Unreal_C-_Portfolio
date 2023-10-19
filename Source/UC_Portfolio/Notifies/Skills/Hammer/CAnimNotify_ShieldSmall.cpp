#include "Notifies/Skills/Hammer/CAnimNotify_ShieldSmall.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "Weapon/Skills/Hammer/CSkill_Hammer_V.h"
#include "Weapon/Skills/Hammer/CSkill_Hammer_V_Shield.h"


FString UCAnimNotify_ShieldSmall::GetNotifyName_Implementation() const
{
	return "Shield_Small";
}

void UCAnimNotify_ShieldSmall::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());

	CheckNull(weapon);
	CheckNull(state);

	if (!!shield)
	{
		//ACSkill_Hammer_V_Shield* Shield = Cast<ACSkill_Hammer_V_Shield>(shield);
		UCSkill_Hammer_V* Shield = Cast<UCSkill_Hammer_V>(shield);
		CheckNull(Shield);
		//CLog::Print(Shield->GetName());


		//Shield->ShieldSmall();
	}

}
