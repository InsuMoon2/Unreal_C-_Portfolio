#pragma once

#include "CoreMinimal.h"
#include "Weapon/CAttachment.h"
#include "CAttachmena_GruxWeapon.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACAttachmena_GruxWeapon : public ACAttachment
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
		class USkeletalMeshComponent* Weapon_Right;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
		class USkeletalMeshComponent* Weapon_Left;

	UPROPERTY(VisibleAnywhere, Category = "Capsule")
		class UCapsuleComponent* Capsule_Right;

	UPROPERTY(VisibleAnywhere, Category = "Capsule")
		class UCapsuleComponent* Capsule_Left;

public:
	ACAttachmena_GruxWeapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
