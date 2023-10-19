#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TargetComponent.generated.h"


UCLASS()
class UC_PORTFOLIO_API UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTargetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:


private:
	void Begin_Targeting();
	void End_Targeting();

	void Change_Target();
	void Tick_Targeting();

public:
	void Change_Focus();

protected:
	UPROPERTY(BlueprintReadOnly , Category = "Component")
		class UWidgetComponent* WidgetComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Component")
		class UCMovementComponent* MovementComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Trace")
		float TraceDistance;

	UPROPERTY(BlueprintReadOnly, Category = "Angle")
		float FinishAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Interp")
		float InterpSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		ACharacter* OwnerCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category = "Character")
		ACharacter* Target;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		ACharacter* Enemy;

	UPROPERTY(BlueprintReadOnly, Category = "Focus")
		bool bMovingFocus;

	// µð¹ö±ë
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;


};
