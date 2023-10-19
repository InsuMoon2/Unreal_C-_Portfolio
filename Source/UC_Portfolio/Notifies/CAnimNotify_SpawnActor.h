#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_SpawnActor.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCAnimNotify_SpawnActor
	: public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY(EditAnywhere)
		class UClass* ActorClass;

	UPROPERTY(EditAnywhere)
		bool InControlRotation;

	UPROPERTY(EditAnywhere)
		float Distance_Forward;

	UPROPERTY(EditAnywhere)
		float Distance_Right;

	UPROPERTY(EditAnywhere)
		float Distance_Up;

	UPROPERTY(EditAnywhere)
		FRotator Spawn_Rotator;

	UPROPERTY(EditAnywhere)
		FVector Spawn_Scale = FVector::OneVector;

	UPROPERTY()
		class ACharacter* OwnerCharacter;
};
