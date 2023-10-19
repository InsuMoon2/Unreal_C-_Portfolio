#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "CSkill_Rolling.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Rolling : public UCSkill
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		FDoActionData SkillData;

public:
	UCSkill_Rolling();

public:
	void BeginPlay
	(ACharacter* InOwner,
		ACAttachment* InAttachment,
		UCDoAction* InDoAction) override;

public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

private:
	class APlayerController* PlayerController;
	class UCAIBehaviorComponent* Behavior;

private:
	FVector MoveToLocation;
	
};
