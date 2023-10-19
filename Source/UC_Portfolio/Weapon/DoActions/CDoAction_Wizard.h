#pragma once

#include "CoreMinimal.h"
#include "Weapon/CDoAction.h"
#include "CDoAction_Wizard.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCDoAction_Wizard : public UCDoAction
{
	GENERATED_BODY()

public:

	UCDoAction_Wizard();

	virtual void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionData,
		const TArray<FHitData>& InHitData,
		const TArray<FHitData>& InGuardData,
		const TArray<FHitData>& InBossHitDatas,
		const TArray<FHitData>& InBossAirHitDatas
	);

	void Tick(float InDeltaTime) override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

private:
	UFUNCTION()
		void OnFireBallOverlap(class AActor* InCauser, class ACharacter* InOtherCharacter);

private:
	class APlayerController* PlayerController;

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACFireBall> fireball;
};
