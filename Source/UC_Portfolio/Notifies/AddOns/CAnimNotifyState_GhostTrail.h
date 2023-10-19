#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_GhostTrail.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCAnimNotifyState_GhostTrail : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
	FString GetNotifyName_Implementation() const override;

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY()
		class ACharacter* OwnerCharacter;

	UPROPERTY()
		class ACGhostTrail* GhostTrail;
};
