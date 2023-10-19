#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CRidingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_PORTFOLIO_API UCRidingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCRidingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
