#include "Notifies/Skills/CAnimNotifyState_SkillAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapon/CSkill.h"

FString UCAnimNotifyState_SkillAction::GetNotifyName_Implementation() const
{
	return "Skill_Action";
}

void UCAnimNotifyState_SkillAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	weapon->GetSkill(Skill_index)->Begin_Skill();
}

void UCAnimNotifyState_SkillAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	weapon->GetSkill(Skill_index)->End_Skill();
}

