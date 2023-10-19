#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CFeetComponent.h"
#include "Components/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		bool Falling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bBow_Aiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
		bool bFeet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
		FFeetData FeetData;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		EWeaponType WeaponType = EWeaponType::UnArmed;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	class ACharacter* OwnerCharacter;

	class UCWeaponComponent* Weapon;
	class UCMovementComponent* Movement;

private:
	FRotator PrevRotation;
	
};
