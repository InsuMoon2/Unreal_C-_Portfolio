#include "Character/Enemies/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/Enemies/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 45;
	Sight->SetMaxAge(2);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Character = Cast<ACCharacter>(InPawn);
	Enemy = Cast<ACEnemy_AI>(InPawn);

	SetGenericTeamId(Enemy->GetTeamID());

	CheckNull(Enemy->GetBehaviorTree());
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	if (actors.Num() > 0)
	{
		for (AActor* target : actors)
		{
			Character = Cast<ACCharacter>(target);

			if (CHelpers::IsSameTeam(Enemy, Character) == false)
			{	
				Blackboard->SetValueAsObject("Target", Character);



				return;
			}
		}

		
	}

	Blackboard->SetValueAsObject("Target", nullptr);
}
