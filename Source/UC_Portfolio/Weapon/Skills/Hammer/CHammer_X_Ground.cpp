#include "Weapon/Skills/Hammer/CHammer_X_Ground.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystem.h"

ACHammer_X_Ground::ACHammer_X_Ground()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Root);
	
	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/GroundAttacks/Fx/Earth/P_EarthGroundAttack2.P_EarthGroundAttack2'");
	Particle->SetTemplate(particle);
}

void ACHammer_X_Ground::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACHammer_X_Ground::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACHammer_X_Ground::OnComponentEndOverlap);

}

void ACHammer_X_Ground::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
	{
		Hitted = character;
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted);
	}
}

void ACHammer_X_Ground::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Hitted = nullptr;
}

