#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkill_Grux_Skill4_Decal.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API ACSkill_Grux_Skill4_Decal : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UDecalComponent* Ground_Decal;

public:	
	ACSkill_Grux_Skill4_Decal();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DecalIncrease();
	virtual void DecalIncrease_Implementation() {}

	void SetOwner(ACharacter* InOwner);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Character")
		ACharacter* OwnerCharacter;
};
