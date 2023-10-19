#pragma once

#include "CoreMinimal.h"
#include "Character/CCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACEnemy
	: public ACCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Color")
		FLinearColor OriginColor = FLinearColor::White;
//private:
//	UPROPERTY(VisibleAnywhere)
//		class USpringArmComponent* SpringArm;
//
//	UPROPERTY(VisibleAnywhere)
//		class UCameraComponent* Camera;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* CrossHair;

public:
	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

private:
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;

public:
	FORCEINLINE UWidgetComponent* GetCrossHair() { return CrossHair; }

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

protected:
	struct FDamageData
	{
		float Power;
		class ACharacter* Character;
		class AActor* Casuer;

		struct FActionDamageEvent* Event;
	};

	FDamageData Damage;

private:
	FTimerHandle RestoreColor_TimerHandle;

public:
	virtual void Landed(const FHitResult& Hit) override;;

protected:
	virtual void Hitted();

public:
	virtual void End_Hitted() override;
	virtual void End_Incapacitation() override;

public:
	UFUNCTION()
		void RestoreColor();

private:
	void Dead();
	void Incapacitation();
	void Execution_Clear();

public:
	void End_Dead() override;

public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	class ACharacter* Attacker;
};


