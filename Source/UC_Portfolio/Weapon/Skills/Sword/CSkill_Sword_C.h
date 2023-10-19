#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Sword_C.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Sword_C : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Skill")
		TSubclassOf<class ACSkill_Sword_C_Trail> SKill_Class;

public:
	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Material")
		class ACAttachment_Sword* Sword;

public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;
};
