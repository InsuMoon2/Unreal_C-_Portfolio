#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Hammer_Jump.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCAnimNotify_Hammer_Jump : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY()
		class ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
		float Jump_Velocity;

public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
