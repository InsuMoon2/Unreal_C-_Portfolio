#include "Behaviors/EQS/CEnvQueryContext_Target.h"
#include "Global.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Character/Enemies/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// 어느것이든 다 받을 수 있어야 해서 UObject로 리턴을 해야한다.
	// Querier가 Character가 될 수도 있고, 다른 것도 될 수도 있기 때문에 최상위형으로 리턴을 하는 것이다.
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	ACAIController* controller = ai->GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject("Target"));

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);

}
