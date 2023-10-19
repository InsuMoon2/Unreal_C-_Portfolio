#include "Weapon/Skills/Hammer/CSkill_Hammer_V_Ground.h"
#include "CSkill_Hammer_V_Explosion.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACSkill_Hammer_V_Ground::ACSkill_Hammer_V_Ground()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Ground", Sphere);


}

void ACSkill_Hammer_V_Ground::BeginPlay()
{
	Super::BeginPlay();

	if (Hitted.Num() > 0)
		Hitted.Empty();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACSkill_Hammer_V_Ground::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACSkill_Hammer_V_Ground::OnComponentEndOverlap);

	if (Ground_TimelineCurve)
	{
		Ground_TimeLineUpdateDelegate.BindUFunction(this, FName("Ground_TimeLineUpdateFunc"));
		Ground_Timeline.AddInterpFloat(Ground_TimelineCurve, Ground_TimeLineUpdateDelegate);

		Ground_TimeLineFinishDelegate.BindUFunction(this, FName("Ground_TimeLineFinishFunc"));
		Ground_Timeline.SetTimelineFinishedFunc(Ground_TimeLineFinishDelegate);
	}

	if (Draw_TimelineCurve)
	{
		Draw_TimeLineUpdateDelegate.BindUFunction(this, FName("Draw_TimeLineUpdateFunc"));
		Draw_Timeline.AddInterpFloat(Draw_TimelineCurve, Draw_TimeLineUpdateDelegate);

		Draw_TimeLineFinishDelegate.BindUFunction(this, FName("Draw_TimeLineFinishFunc"));
		Draw_Timeline.SetTimelineFinishedFunc(Draw_TimeLineFinishDelegate);

		ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda([=]()
			{
				Draw_Timeline.PlayFromStart();
			});

		player->GetWorld()->GetTimerManager().SetTimer(Draw_TimerHandle, timerDelegate, DrawTime, false);
	}


}

void ACSkill_Hammer_V_Ground::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Ground_TimelineCurve)
	{
		Ground_Timeline.TickTimeline(DeltaTime);
	}

	if (Draw_TimelineCurve)
	{
		Draw_Timeline.TickTimeline(DeltaTime);
	}
}

void ACSkill_Hammer_V_Ground::GroundSmall(ACharacter* InOwner)
{
	Ground_Timeline.PlayFromStart();

	Owner = InOwner;
}

void ACSkill_Hammer_V_Ground::Ground_TimeLineUpdateFunc(float Output)
{
	FVector lerp = UKismetMathLibrary::VLerp(LerpA, LerpB, Output);

	Sphere->SetRelativeScale3D(lerp);
}

void ACSkill_Hammer_V_Ground::Ground_TimeLineFinishFunc()
{
	if (!!Sphere)
	{
		Hitted.Empty();
		//GetWorld()->GetTimerManager().ClearTimer(Ground_TimerHandle);
	}

}

void ACSkill_Hammer_V_Ground::Draw_TimeLineUpdateFunc(float Output)
{
	LerpTime = Output;

	if (Hitted.Num() > 0)
	{
		for (int32 i = 0; i <= Hitted.Num() - 1; i++)
		{
			if (Hitted[i] != Owner)
			{
				FVector location = Hitted[i]->GetActorLocation();
				FVector Unit = UKismetMathLibrary::GetDirectionUnitVector(Owner->GetActorLocation(), location) * LerpDistance;
				Unit += Owner->GetActorLocation();

				FVector Lerp = UKismetMathLibrary::VLerp(location, Unit, LerpTime);

				float distance = Hitted[i]->GetDistanceTo(Owner);
				//CLog::Print(distance);

				ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(Hitted[i]);
				if (!!boss)
					Hitted[i]->PlayAnimMontage(Boss_DrawOut_Montage);

				else
					Hitted[i]->PlayAnimMontage(DrawOut_Montage);

				if (distance >= LerpDistance)
					Hitted[i]->SetActorLocation(Lerp);

			}
		}

	}
}

void ACSkill_Hammer_V_Ground::Draw_TimeLineFinishFunc()
{
	GetWorld()->GetTimerManager().ClearTimer(Draw_TimerHandle);
	this->Destroy();
}

void ACSkill_Hammer_V_Ground::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckTrue(player == character);

	CheckTrue(CHelpers::IsSameTeam(player, character));

	if (character != player)
		Hitted.AddUnique(character);

	//if (Hitted.Num() > 0)
	//{
	//	for (int32 i = 0; i < Hitted.Num() - 1; i++)
	//	{
	//		//CLog::Print(Hitted[i]->GetName());
	//		//HitData.SendDamage(player, this, Hitted[i]);
	//	}
	//}
}

void ACSkill_Hammer_V_Ground::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Hitted.Empty();
}


