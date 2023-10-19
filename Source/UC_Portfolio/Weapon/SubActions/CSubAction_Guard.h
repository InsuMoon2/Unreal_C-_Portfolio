#pragma once

#include "CoreMinimal.h"
#include "Weapon/CSubAction.h"
#include "Weapon/CWeaponStructures.h"
#include "CSubAction_Guard.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSubAction_Guard : public UCSubAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Action")
		FDoActionData ActionData;

	UPROPERTY(EditAnywhere, Category = "Action")
		TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Guard")
		class UAnimMontage* Guard_End;

public:
	void Pressed() override;
	void Released() override;

	void Begin_SubAction_Implementation() override;
	void End_SubAction_Implementation() override;

private:
	TArray<class ACharacter*> Hitted;

private:
	UFUNCTION()
		void OnAttachmentEndCollision();

	UFUNCTION()
		void OnAttachmentBeginOverlap
	(
		class ACharacter* InAttacker,
		AActor* InAttackCauser,
		class ACharacter* InOther
	);

	
	
};
