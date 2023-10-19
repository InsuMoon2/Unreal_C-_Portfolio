#include "Weapon/Skills/Grux/CSkill3_Spawn.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"


ACSkill3_Spawn::ACSkill3_Spawn()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle Component", Root);

	Sphere->SetHiddenInGame(false);
	Sphere->SetVisibility(true);

	
}

void ACSkill3_Spawn::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(1.0f);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACSkill3_Spawn::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACSkill3_Spawn::OnComponentEndOverlap);
}

void ACSkill3_Spawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//if (!!GetOwner())
	//	CLog::Print(Cast<ACharacter>(GetOwner()));
	
}

void ACSkill3_Spawn::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//CLog::Print("Hit");
	//CLog::Print(GetOwner());

	CheckNull(GetOwner());
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
	{
		Hitted = character;
		HitData.SendDamage(Cast<ACCharacter>(GetOwner()), this, Hitted);
	}
}

void ACSkill3_Spawn::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Hitted = nullptr;
}

