#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Hammer_X.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Hammer_X : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Skill")
		TSubclassOf<class ACSkill_Hammer_X_Ground> SKill_Class;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		FVector SKill_Location;

private:
	UPROPERTY(EditAnywhere, Category = "Skill")
		FDoActionData SkillData;


public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

};
