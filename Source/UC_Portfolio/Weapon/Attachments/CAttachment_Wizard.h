#pragma once

#include "CoreMinimal.h"
#include "Weapon/CAttachment.h"
#include "CAttachment_Wizard.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACAttachment_Wizard : public ACAttachment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Capsule")
		class UCapsuleComponent* Capsule;

public:
	ACAttachment_Wizard();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
