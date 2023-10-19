#pragma once

#include "CoreMinimal.h"
#include "Weapon/CAttachment.h"
#include "CAttachment_Hammer.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACAttachment_Hammer : public ACAttachment
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = "SkeletalMesh")
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Capsule")
		class UCapsuleComponent* Capsule;

public:
	ACAttachment_Hammer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
