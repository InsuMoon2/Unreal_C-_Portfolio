#include "Weapon/Skills/Hammer/CSkill_Hammer_V_Shield.h"
#include "Global.h"

ACSkill_Hammer_V_Shield::ACSkill_Hammer_V_Shield()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Shield_Sphere, "Shield", Root);

	TimelineCurve = nullptr;
}

void ACSkill_Hammer_V_Shield::BeginPlay()
{
	Super::BeginPlay();

	if (TimelineCurve)
	{
		TimeLineUpdateDelegate.BindUFunction(this, FName("TimeLineUpdateFunc"));
		Timeline.AddInterpFloat(TimelineCurve, TimeLineUpdateDelegate);

		TimeLineFinishDelegate.BindUFunction(this, FName("TimeLineFinishFunc"));
		Timeline.SetTimelineFinishedFunc(TimeLineFinishDelegate);

		//float Min = 0, Max = 0;
		//TimelineCurve->GetTimeRange(Min, Max);
		//Timeline.SetTimelineLength(Max);

		
		//ShieldSmall();
	}

	
}

void ACSkill_Hammer_V_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);
}

void ACSkill_Hammer_V_Shield::ShieldSmall(ACharacter* InOwner)
{
	Owner = InOwner;

	Timeline.PlayFromStart();
}

void ACSkill_Hammer_V_Shield::TimeLineUpdateFunc(float Output)
{
	FVector lerp = UKismetMathLibrary::VLerp(LerpA, LerpB, Output);
	//CLog::Print(lerp);

	Shield_Sphere->SetRelativeScale3D(lerp);
}

void ACSkill_Hammer_V_Shield::TimeLineFinishFunc()
{
	if (!!Shield_Sphere)
		this->Destroy();

}

