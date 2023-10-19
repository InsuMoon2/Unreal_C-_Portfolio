#include "CSkill_Sword_X_Trail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACSkill_Sword_X_Trail::ACSkill_Sword_X_Trail()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "ProjectileMovement");
	Projectile->SetAutoActivate(true);
	Projectile->InitialSpeed = 500.0f;
	Projectile->ProjectileGravityScale = 0.0f;

	UNiagaraSystem* niagara;
	CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/Sword_Skill2_2.Sword_Skill2_2'");
	Niagara->SetAsset(niagara);
}

void ACSkill_Sword_X_Trail::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACSkill_Sword_X_Trail::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACSkill_Sword_X_Trail::OnComponentEndOverlap);
	
}

void ACSkill_Sword_X_Trail::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);
	ACharacter* character = Cast<ACharacter>(OtherActor);

	CheckNull(character);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckTrue(state->IsDeadMode());

	CheckTrue(CHelpers::IsSameTeam(Cast<ACharacter>(GetOwner()), character));

	if (!!character)	
	{
		ACharacter* Boss = Cast<ACEnemy_Boss>(character);
		if (!!Boss)
		{
			Hitted.AddUnique(Boss);
			BossHitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Boss);
			
			return;
		}

		Hitted.AddUnique(character);
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, character);

		return;
	}
}

void ACSkill_Sword_X_Trail::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
		Hitted.Remove(character);

	ACharacter* Boss = Cast<ACEnemy_Boss>(character);
	if (!!Boss)
		Hitted.Remove(character);
}


