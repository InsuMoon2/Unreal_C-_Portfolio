#include "Character/Enemies/CAIController_Boss.h"
#include "CEnemy_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController_Boss::ACAIController_Boss()
{
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 1500;
	Sight->LoseSightRadius = 2000;
	Sight->PeripheralVisionAngleDegrees = 90;
	Sight->SetMaxAge(5);

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController_Boss::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController_Boss::OnPerceptionUpdated);
}

void ACAIController_Boss::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Boss = Cast<ACEnemy_Boss>(InPawn);
	SetGenericTeamId(Boss->GetTeamID());

	CheckNull(Boss->GetBehaviorTree());
	UseBlackboard(Boss->GetBehaviorTree()->BlackboardAsset, Blackboard);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Boss);
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Boss->GetBehaviorTree());

}

void ACAIController_Boss::OnUnPossess()
{
	Super::OnUnPossess();

}

void ACAIController_Boss::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	if(actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);

		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);
}


