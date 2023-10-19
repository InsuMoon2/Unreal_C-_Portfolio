#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "CSkill_GruxSkill4.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_GruxSkill4 : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		TArray<FHitData> HitDatas;

public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

private:
	TArray<class ACharacter*> Hitted;
	int32 HitIndex;

	ACharacter* TraceCharacter;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
		void OnAttachmentEndCollision();

public:
	void MoveTo(ACharacter* InTarget);

public:
	UPROPERTY(EditAnywhere, Category = "Decal")
		TSubclassOf<class ACSkill_Grux_Skill4_Decal> Decal;

private:
	UPROPERTY(EditAnywhere, Category = "Decal")
		float decalZ = 20.0f;

	
};
