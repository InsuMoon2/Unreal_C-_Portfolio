#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Upper.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Upper : public UCSkill
{
	GENERATED_BODY()

public:
	// 맞은 놈이 있는가
	FORCEINLINE bool IsExistHitted() { return Hitted.Num() > 0; }
	FORCEINLINE void SetHittedEmpty() { return Hitted.Empty(); }

	FORCEINLINE bool GetHitted() {return bhit; }
	FORCEINLINE void SetHitted(bool InHit) { bhit = InHit; }

private:
	UPROPERTY(EditAnywhere, Category = "Upper_Action")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Upper_Action")
		TArray<FHitData> HitData;

	UPROPERTY(EditAnywhere, Category = "Upper_Action")
		TArray<FHitData> BossHitData;

	UPROPERTY(EditAnywhere, Category = "Upper_Action")
		bool ChangeCheck = true;

public:
	void Skill_Upper() override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

private:
	TArray<class ACharacter*> Hitted;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
		void OnAttachmentEndCollision();

private:
	bool bhit = false;
	
};
