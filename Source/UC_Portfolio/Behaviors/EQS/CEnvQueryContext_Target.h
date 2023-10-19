#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEnvQueryContext_Target.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCEnvQueryContext_Target : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	
};
