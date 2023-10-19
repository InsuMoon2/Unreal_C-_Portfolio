#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "CSkill_Parrying.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Parrying : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Parrying")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Parrying")
		class USoundCue* Sound;

	UPROPERTY(EditAnywhere, Category = "Parrying_Hitted")
		TArray<FHitData> HitData;

	UPROPERTY(EditAnywhere, Category = "Parrying_Hitted")
		TArray<FHitData> Boss_HitData;


public:
	void Skill_Parrying() override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

	void HitStop();

private:
	TArray<class ACharacter*> Hitted;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
		void OnAttachmentEndCollision();

private:
	UPROPERTY(EditAnywhere, Category = "Parrying")
		float StopTime;

};
