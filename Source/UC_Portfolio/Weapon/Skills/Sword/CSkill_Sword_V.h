#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/CSkill.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Sword_V.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_Sword_V : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float Distance = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float Speed = 200.0f;
		
	// µð¹ö±ë
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FHitData HitData;

public:
	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		TArray<FHitData> BossHitDatas;

public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

	void Begin_Skill_Implementation() override;
	void End_Skill_Implementation() override;

private:
	bool bMoving;

	FVector Start;
	FVector End;

private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
		void OnAttachmentEndCollision();

private:
	TArray<class ACharacter*> Hitted;
	TArray<class ACharacter*> Overlapped;

	int32 HitIndex;


};
