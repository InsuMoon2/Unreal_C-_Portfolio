#include "Weapon/Skills/Hammer/CSkill_Hammer_X_Ground.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Particles/ParticleSystem.h"

ACSkill_Hammer_X_Ground::ACSkill_Hammer_X_Ground()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Ground", Root);

	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/GroundAttacks/Fx/Earth/P_EarthGroundAttack2.P_EarthGroundAttack2'");
	Particle->SetTemplate(particle);
	Particle->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

}

void ACSkill_Hammer_X_Ground::BeginPlay()
{
	Super::BeginPlay();

	//SetLifeSpan(0.5f);

	// Particle捞 场朝 锭 角青
	Particle->OnSystemFinished.AddDynamic(this, &ACSkill_Hammer_X_Ground::OnParticleFinished);

	// 面倒贸府
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACSkill_Hammer_X_Ground::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACSkill_Hammer_X_Ground::OnComponentEndOverlap);
	
}

void ACSkill_Hammer_X_Ground::OnParticleFinished(UParticleSystemComponent* FinishedComponent)
{
	Destroy();
}

void ACSkill_Hammer_X_Ground::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
	{
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
		CheckTrue(state->IsDeadMode());

		CheckTrue(CHelpers::IsSameTeam(Cast<ACharacter>(GetOwner()), character));

		ACharacter* Boss = Cast<ACEnemy_Boss>(character);
		if (!!Boss)
		{
			Hitted = Boss;
			BossHitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted);

			return;
		}

		Hitted = character;
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted);
	}
}

void ACSkill_Hammer_X_Ground::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Hitted = nullptr;
}

