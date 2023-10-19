#pragma once

#include "CoreMinimal.h"
#include "Weapon/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

	FORCEINLINE TArray<FDoActionData> GetDoActionDats() { return DoActionDatas; }
	FORCEINLINE bool SetExistCombo(bool InExist) { return bExist = InExist; }
	FORCEINLINE bool GetExistCombo() { return bExist; }

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Tick(float InDeltaTime) override;

public:
	void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther) override;
	void OnAttachmentEndCollision() override;

	void Change_Focus(AController* Incontroller, FRotator InTarget);

private:

	bool bEnable;
	bool bExist;

private:
	TArray<class ACharacter*> Hitted;

private:
	FRotator rotator;
	FRotator target;
	AController* OwnerController;

private:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Parrying;
};
