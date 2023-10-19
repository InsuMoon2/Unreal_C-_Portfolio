#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_BackStep.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCAnimNotify_BackStep : public UAnimNotify
{
	GENERATED_BODY()

private:
private:
	UPROPERTY(EditAnywhere, Category = "Launch")
		FVector Launch = FVector(0, 0, 0.0f);

public:
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
