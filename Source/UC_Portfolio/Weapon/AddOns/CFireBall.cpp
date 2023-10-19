#include "Weapon/AddOns/CFireBall.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACFireBall::ACFireBall()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Capsule", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

}

void ACFireBall::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACFireBall::OnComponentBeginOverlap);
}

void ACFireBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACFireBall::Shoot(const FVector& InForward)
{
	Projectile->Velocity = Projectile->InitialSpeed * InForward;
	Projectile->SetActive(true);

}

void ACFireBall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherCharacter = Cast<ACharacter>(OtherActor);
	CheckTrue(GetOwner() == OtherCharacter);
	//SetLifeSpan(LifeSpanAfterCollision);

	if (!!OtherCharacter && OnOverlap.IsBound())
		OnOverlap.Broadcast(this, OtherCharacter);
}


void ACFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

