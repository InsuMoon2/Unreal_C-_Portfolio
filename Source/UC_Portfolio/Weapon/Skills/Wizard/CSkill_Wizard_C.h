#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Wizard_C.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Wizard_C : public UCSkill
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		FDoActionData SkillData;

public:
	UCSkill_Wizard_C();

	void BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

private:
	TArray<class ACharacter*> Hitted;

};
