#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetingComponent.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Toggle_Target();

	void Change_Focus(bool InRight);
	void Right_Focus();
	void Left_Focus();

private:
	void Begin_Targeting();
	void End_Targeting();

	void Change_Target(ACharacter* InCandidate);
	void Tick_Targeting();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class UWidgetComponent* CrossHair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class UCMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class UCCamComponent* CamComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class UCStatusComponent* StatusComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character")
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		class ACharacter* Target;

	UPROPERTY(BlueprintReadOnly, Category = "Distance")
		float TraceDistance = 1500.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Angle")
		float FinishAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Interp")
		float InterpSpeed = 5.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Focus")
		bool bMovingFocus;

	// µð¹ö±ë
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
	bool bTargetingCheck = false;

};

