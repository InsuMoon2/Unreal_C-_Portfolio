#include "Weapon/Skills/Hammer/CSkill_Hammer_V_Explosion.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Particles/ParticleSystem.h"

ACSkill_Hammer_V_Explosion::ACSkill_Hammer_V_Explosion()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Explosion", Box);

}

void ACSkill_Hammer_V_Explosion::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// 충돌처리
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACSkill_Hammer_V_Explosion::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACSkill_Hammer_V_Explosion::OnComponentEndOverlap);
	
}

void ACSkill_Hammer_V_Explosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSkill_Hammer_V_Explosion::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);
	ACharacter* character = Cast<ACharacter>(OtherActor);

	CheckTrue(CHelpers::IsSameTeam(Cast<ACharacter>(GetOwner()), character));

	if (!!character)
	{
		Hitted = character;
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted);
	}
	
	
}

void ACSkill_Hammer_V_Explosion::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Hitted = nullptr;
}

