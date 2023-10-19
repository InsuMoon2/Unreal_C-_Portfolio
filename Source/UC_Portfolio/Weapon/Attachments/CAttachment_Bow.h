#pragma once

#include "CoreMinimal.h"
#include "Weapon/CAttachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

private:
	// View Pitch °¢ Á¶Àý
	UPROPERTY(EditDefaultsOnly, Category = "View")
		FVector2D ViewPitchRange = FVector2D(-40.0f, +30.0f);

private:
	UPROPERTY(VisibleAnywhere, Category = "Poseable")
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Poseable")
		class UPoseableMeshComponent* PoseableMesh;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* QuiverMesh;

public:
	ACAttachment_Bow();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void OnBeginEquip_Implementation() override;
	void OnUnequip_Implementation() override;

public:
	float* GetBend();

private:
	FVector2D OriginViewPitchRange;

};
