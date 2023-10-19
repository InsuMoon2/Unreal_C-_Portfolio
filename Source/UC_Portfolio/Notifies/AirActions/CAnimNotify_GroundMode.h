#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_GroundMode.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCAnimNotify_GroundMode : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "AirCombo")
		FVector Launch = FVector(0, 0, 0.0f);

public:

	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
