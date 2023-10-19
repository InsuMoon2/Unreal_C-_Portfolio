#include "Weapon/Skills/Sword/CSkill_Sword_C_Trail.h"
#include "Global.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACSkill_Sword_C_Trail::ACSkill_Sword_C_Trail()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Fire, "Fire", Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Fire_Ground, "Fire_Ground", Root);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "ProjectileMovement");
	Projectile->SetAutoActivate(true);
	Projectile->InitialSpeed = 1000.0f;
	Projectile->ProjectileGravityScale = 0.0f;

	// Fire
	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/AdvancedMagic/AdvancedMagicFX09/Particles/P_ky_flame.P_ky_flame'");
	Fire->SetTemplate(particle);
	// FireGround
	UParticleSystem* particle2;
	CHelpers::GetAsset<UParticleSystem>(&particle2, "ParticleSystem'/Game/GroundAttacks/Fx/Earth/Sword_Ground_Attack.Sword_Ground_Attack'");
	Fire_Ground->SetTemplate(particle2);
}

void ACSkill_Sword_C_Trail::BeginPlay()
{
	Super::BeginPlay();

	// Particle捞 场朝 锭 角青
	//Fire->OnSystemFinished.AddDynamic(this, &ACSkill_Sword_C_Trail::OnParticleFinished);
	Fire_Ground->OnSystemFinished.AddDynamic(this, &ACSkill_Sword_C_Trail::OnParticleFinished);

	//面倒 贸府
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACSkill_Sword_C_Trail::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACSkill_Sword_C_Trail::OnComponentEndOverlap);

	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
		{
			for (int32 i = Hitted.Num() - 1; i >= 0; i--)
				HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
		});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, DamageInterval, true, 0);
}

void ACSkill_Sword_C_Trail::OnParticleFinished(UParticleSystemComponent* FinishedComponent)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	Destroy();
}

void ACSkill_Sword_C_Trail::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
			Hitted.AddUnique(Boss);
			BossHitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[0]);

			return;
		}

		Hitted.AddUnique(character);
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[0]);

		return;
	}
}

void ACSkill_Sword_C_Trail::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
		Hitted.Remove(character);

	ACharacter* Boss = Cast<ACEnemy_Boss>(character);
	if (!!Boss)
		Hitted.Remove(character);

}




