#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGameUIComponent.generated.h"


UCLASS()
class UC_PORTFOLIO_API UCGameUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCGameUIComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
