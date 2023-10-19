#pragma once

#include "CoreMinimal.h"
#include "CDoAction_Combo.h"
#include "CDoAction_AirCombo.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCDoAction_AirCombo : public UCDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

	FORCEINLINE bool SetExistCombo(bool InExist) { return bExist = InExist; }
	FORCEINLINE bool GetExistCombo() { return bExist; }

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void Tick(float InDeltaTime) override;

public:
	void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther) override;
	void OnAttachmentEndCollision() override;

public:
	void GravityVelocityChange(
		ACharacter* InCharacter, float InGravity = 1.0f,
		FVector InVelocity = FVector::ZeroVector);

private:
	int32 Index = 0;

	bool bEnable;
	bool bExist;

private:
	TArray<class ACharacter*> Hitted;

private:
	FRotator rotator;
	FRotator target;
	AController* controller;

private:
	FTimerHandle Gravity_TimerHandle;
};
