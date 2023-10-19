#include "Weapon/Skills/Grux/CSkill_GruxSkill3.h"
#include "CSkill3_Spawn.h"
#include "CSkill_Grux_Skill4_Decal.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CDoAction.h"

UCSkill_GruxSkill3::UCSkill_GruxSkill3()
{
	
}

void UCSkill_GruxSkill3::Skill_Pressed(ESkillType InSkillIndex)
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSkillMode());

	Super::Skill_Pressed(ESkillType::Skill1);

	State->SetActionMode();
	State->OnSkillMode();

	SkillData.DoAction(Owner);

	if (!!Niagara)
	{
		FTransform transform;
		FVector location = (FVector(
			Owner->GetActorLocation().X, Owner->GetActorLocation().Y,
			Owner->GetActorLocation().Z - 88.0f));

		transform.SetLocation(location);
		transform.SetRotation(FQuat(Owner->GetActorRotation()));

		CHelpers::PlayEffect(Owner->GetWorld(), Niagara, transform);
	}
}

void UCSkill_GruxSkill3::Skill_Released(ESkillType InSkillIndex)
{
	Super::Skill_Released(InSkillIndex);

}

void UCSkill_GruxSkill3::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

}

void UCSkill_GruxSkill3::Begin_Skill_Implementation()
{
	CreateCircle();


}

void UCSkill_GruxSkill3::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();


	State->SetIdleMode();
	State->OffSkillMode();

	Movement->Move();
	HitIndex = 0;

	if (TimerHandle.IsValid())
		Owner->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

}

void UCSkill_GruxSkill3::CreateCircle()
{
	float Range = 1200.0f;
	int32 NumPoints = 15;
	SpawnPoints.Empty();

	FVector CharacterLocation = Owner->GetActorLocation();
	float AngleBetweenPoints = 360.0f / NumPoints;

	for (int32 i = 0; i < NumPoints; i++)
	{
		float RandomX = FMath::RandRange(50.0f, Range);
		float RandomY = FMath::RandRange(50.0f, Range);

		float CurrentAngle = i * AngleBetweenPoints;
		float X = FMath::Cos(FMath::DegreesToRadians(CurrentAngle)) * RandomX;
		float Y = FMath::Sin(FMath::DegreesToRadians(CurrentAngle)) * RandomY;

		//FVector SpawnPoint = CharacterLocation + FVector(X, Y,  + 210.0f);
		FVector SpawnPoint = CharacterLocation + FVector(X, Y, -SpawnZ);
		SpawnPoints.Add(SpawnPoint);
	}

	//CLog::Print(CharacterLocation);

	SetUpRandomSpawnTimer();
}

void UCSkill_GruxSkill3::SetUpRandomSpawnTimer()
{
	float MinDelay = 0.1f;
	float MaxDelay = 2.5f;

	for (const FVector& SpawnPoint : SpawnPoints)
	{
		float RandomDelay = FMath::RandRange(MinDelay, MaxDelay);
		FTimerHandle UnusedHandle;

		Owner->GetWorld()->GetTimerManager().SetTimer(
			UnusedHandle,
			[this, SpawnPoint]()
			{
				SpawnRandomActor(SpawnPoint);
			},
			RandomDelay, false);
	}
}

void UCSkill_GruxSkill3::SpawnRandomActor(FVector SpawnLocation)
{
	FTransform transform;
	transform.SetLocation(FVector(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z + decalZ));
	transform.SetRotation(FQuat(FRotator(FRotator::ZeroRotator)));
	transform.SetScale3D(FVector(0.5f, 0.5f, 0.5f));

	ACSkill_Grux_Skill4_Decal* SpawnedActor =
		Owner->GetWorld()->SpawnActor<ACSkill_Grux_Skill4_Decal> (Spawn_Decal, transform);

	SpawnedActor->SetOwner(Owner);

	if (!!SpawnedActor)
	{
		// TODO :: Actor 추가작업
		//SpawnedActor->SetOwner(Owner);

	}
}


