#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "CSkill_Hammer_V.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Hammer_V : public UCSkill
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Skill_Spawn")
		TSubclassOf<class ACSkill_Hammer_V_Shield> Spawn_Shield_Class;

	UPROPERTY(EditAnywhere, Category = "Skill_Spawn")
		TSubclassOf<class ACSkill_Hammer_V_Ground> Spawn_Ground_Class;

	UPROPERTY(EditAnywhere, Category = "Skill_Spawn")
		TSubclassOf<class ACSkill_Hammer_V_Explosion> Spawn_Explosion_Class;

public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

private:
	class ACSkill_Hammer_V_Shield* Shield;
	class ACSkill_Hammer_V_Ground* Ground;
	class ACSkill_Hammer_V_Explosion* Explosion;

	UPROPERTY(EditAnywhere, Category = "Spawn_Scale")
		FVector Shield_Scale;

	UPROPERTY(EditAnywhere, Category = "Spawn_Scale")
		FVector Ground_Scale;

	UPROPERTY(EditAnywhere, Category = "Spawn_Scale")
		FVector Explosion_Scale;
		
};
