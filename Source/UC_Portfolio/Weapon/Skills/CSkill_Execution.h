#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Execution.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Execution : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Execution")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Execution")
		TArray<FHitData> HitData;

public:
	void Skill_Execution() override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

private:
	TArray<class ACharacter*> Hitted;
	int32 HitIndex = 0;

private:
	UPROPERTY(EditAnywhere, Category = "Montage")
		class UAnimMontage* ExecutionMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
		class UAnimMontage* Grux_ExecutionMontage;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
		void OnAttachmentEndCollision();
	
};
