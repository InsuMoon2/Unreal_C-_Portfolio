#pragma once

#include "CoreMinimal.h"
#include "Weapon/CAttachment.h"
#include "CAttachment_Sword.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACAttachment_Sword : public ACAttachment
{
	GENERATED_BODY()

public:
	FORCEINLINE class UNiagaraComponent* GetNaiagara() { return Niagara; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SkeletalMesh")
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Capsule")
		class UCapsuleComponent* Capsule;

public:
	ACAttachment_Sword();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Material")
		void ChangeFireMaterial();

	UFUNCTION(BlueprintCallable, Category = "Material")
		void ChangeMaterialReset();

private:
	UMaterialInterface* FireMaterial;
	UMaterialInterface* OriginMaterial;

private:
	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Niagara;
};
