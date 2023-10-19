#pragma once

#include "CoreMinimal.h"

#include "ActorSequenceComponent.h"
#include "Character/CCharacter.h"

#include "Components/CStateComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Parkour/CParkourComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACPlayer
	: public ACCharacter

{
	GENERATED_BODY()

private:
	/*UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 1;*/

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* SequenceCam;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* MainCamChild;

	UPROPERTY(VisibleAnywhere)
		class UChildActorComponent* SequenceChild;

	// Mini Map
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SceneCaptureArm;

private:
	//UPROPERTY(VisibleAnywhere)
	//	class UActorSequenceComponent* ActorSequence;

private:
	// Component
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCCamComponent* CameraControl;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCZoomComponent* Zoom;

	UPROPERTY(VisibleDefaultsOnly)
		class UCParkourComponent* Parkour;

	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetingComponent* Targeting;

	UPROPERTY(VisibleDefaultsOnly)
		class UCInGameUIComponent* InGameUI;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* ArrowGroup;

	UPROPERTY(EditAnywhere)
		TArray<class UArrowComponent*> Arrows;

public:
	//FORCEINLINE UActorSequenceComponent* GetActorSequence() { return ActorSequence; }
	FORCEINLINE void SetRolling(bool InRolling) { IsRolling = InRolling; }

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Landed(const FHitResult& Hit) override;
	void OnAvoid();

private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Character;
		class AActor* Casuer;

		struct FActionDamageEvent* Event;
	};

	FDamageData Damage;

public:
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void Hitted();

private:
	UFUNCTION()
		void CanMoving();

	FTimerHandle Moving_TimerHandle;

private:
	void Jump() override;
	void Roll();
	void BackStep();

public:
	// Notify에서 호출해줄 것이다.
	void End_BackStep() override;
	void End_Hitted() override;
	void End_Guard() override;

private:
	// AirCombo
	void AirAction();

	// Parkour
	void Do_Parkour();

private:
	// Zoom
	void SetZooming(float InValue);

public:
	// SubAction
	void OnRightButton();
	void OffRightButton();

public:
	// Camera
	AActor* IsMain_Camera_Child();
	AActor* IsSequence_Child();

private:
	UPROPERTY(EditAnywhere, Category = "OnAvoid")
		float OnAvoidMana = 15.0f;

private:
	bool bJump = true;
	int double_Jump = 0;

private:
	FVector  InputDirection;
	FRotator RollDirection;

	FRotator RollRotation;

private:
	bool IsRolling = false;

};
