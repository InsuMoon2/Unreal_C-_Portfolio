#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSkill.h"
#include "CSkill_GruxSkill3.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSkill_GruxSkill3 : public UCSkill
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		FDoActionData SkillData;

	UPROPERTY(EditAnywhere, Category = "Skill_Action")
		TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Niagara")
		class UNiagaraSystem* Niagara;

public:
	UCSkill_GruxSkill3();

public:
	void Skill_Pressed(ESkillType InSkillIndex) override;
	void Skill_Released(ESkillType InSkillIndex) override;

	void Tick_Implementation(float InDeltaTime) override;

private:
	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;

private:
	TArray<class ACharacter*> Hitted;
	int32 HitIndex;

public:
	void CreateCircle();

	UFUNCTION()
		void SetUpRandomSpawnTimer();

	UFUNCTION()
		void SpawnRandomActor(FVector SpawnLocation);

private:
	UPROPERTY(EditAnywhere, Category = "Spawn")
		UClass* Spawn_Class;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		float SpawnZ = 130.0f;

	TArray<FVector> SpawnPoints;

	FTimerHandle TimerHandle;

public:
	UPROPERTY(EditAnywhere, Category = "Spawn_Decal")
		TSubclassOf<class ACSkill_Grux_Skill4_Decal> Spawn_Decal;

private:
	UPROPERTY(EditAnywhere, Category = "Spawn_Decal")
		float decalZ = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn_Decal")
		FVector decalScale;
};
