#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Dagger_Z.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Dagger_Z : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Skill_Class")
		TSubclassOf<class ACSkill_Dagger_Z_Throw> SKill_Class;

	UPROPERTY(EditDefaultsOnly, Category = "Skill_Class")
		FVector SKill_Location;

	UPROPERTY(EditAnywhere, Category = "Skill_Data")
		FDoActionData SkillData;

public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;
};
