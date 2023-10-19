#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk = 0, Run, Sprint, BossRun, MAX
};

UCLASS()
class UC_PORTFOLIO_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ESpeedType::MAX] = { 200, 400, 600, 800 };

protected:
	virtual void BeginPlay() override;

private:
	bool IsHittedMode();
	bool IsGuardMode();

public:
	// Moving
	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE void Move()    { bCanMove = true; }
	FORCEINLINE void Stop()    { bCanMove = false; }

	// Speed
	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }
	FORCEINLINE float GetBossSpeed() { return Speed[(int32)ESpeedType::BossRun]; }


public:
	void SetSpeed(ESpeedType InType);

public:
	void OnWalk();
	void OnRun();
	void OnSprint();
	void OnBossSpeed();

public:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);

private:
	// 캐릭터 세팅
	class ACharacter* OwnerCharacter;

private:
	bool bCanMove = true;
};
