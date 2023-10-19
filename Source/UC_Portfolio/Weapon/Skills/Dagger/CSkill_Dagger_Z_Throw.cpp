#include "Weapon/Skills/Dagger/CSkill_Dagger_Z_Throw.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACSkill_Dagger_Z_Throw::ACSkill_Dagger_Z_Throw()
{
	// Setting
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Box", Root);

	// Projectile
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "ProjectileMovement");
	Projectile->SetAutoActivate(true);
	Projectile->InitialSpeed = 500.0f;
	Projectile->ProjectileGravityScale = 0.0f;

}

void ACSkill_Dagger_Z_Throw::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACSkill_Dagger_Z_Throw::OnComponentBeginOverlap);
	//Capsule->OnComponentEndOverlap.AddDynamic(this, &ACSkill_Dagger_Z_Throw::OnComponentEndOverlap);
}

void ACSkill_Dagger_Z_Throw::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
	{
		Hitted = character;
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted);
	}
}

void ACSkill_Dagger_Z_Throw::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Hitted = nullptr;
}

