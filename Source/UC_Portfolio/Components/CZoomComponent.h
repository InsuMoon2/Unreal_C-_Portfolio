#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CZoomComponent.generated.h"


UCLASS()
class UC_PORTFOLIO_API UCZoomComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Zooming")
		float Speed = 20;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		float InterpSpeed = 5;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		float MinRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Zooming")
		float MaxRange = 500.0f;

public:	
	UCZoomComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetZoomValue(float InValue);

private:
	class USpringArmComponent* SpringArm;

	float CurrentValue;

};
