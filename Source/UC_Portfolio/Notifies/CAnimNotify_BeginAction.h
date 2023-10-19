#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Weapon/CWeaponStructures.h"
#include "CAnimNotify_BeginAction.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCAnimNotify_BeginAction : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EActionType ActionType = EActionType::Action;

public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;



};
