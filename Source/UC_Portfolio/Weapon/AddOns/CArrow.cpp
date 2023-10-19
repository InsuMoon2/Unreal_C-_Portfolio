#include "Weapon/AddOns/CArrow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"

ACArrow::ACArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", RootComponent);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Capsule);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Trail", StaticMesh);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	UStaticMesh* static_Mesh;
	CHelpers::GetAsset(&static_Mesh, "StaticMesh'/Game/Character/Weapons/ElvenBow/SM_ElvenArrow.SM_ElvenArrow'");
	StaticMesh->SetStaticMesh(static_Mesh);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UParticleSystem* particle;
	CHelpers::GetAsset(&particle, "ParticleSystem'/Game/_Game/Weapon/Bow/Trail/P_Arrow_Particle.P_Arrow_Particle'");
	Particle->SetTemplate(particle);

	Projectile->InitialSpeed = 1000.f;
	Projectile->ProjectileGravityScale = 0;
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;
	Capsule->SetCapsuleRadius(8.0f);
	Capsule->SetCapsuleHalfHeight(8.0f);
	//Capsule->SetCollisionProfileName("BlockAll");

	
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	//Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACArrow::OnComponentBeginOverlap);
	Projectile->Deactivate();
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Capsule->OnComponentHit.RemoveDynamic(this, &ACArrow::OnComponentHit);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);

}

void ACArrow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ACArrow::Shoot(const FVector& InForward, ACharacter* InCharacter)
{
	//Projectile->Velocity = InForward * Projectile->InitialSpeed;

	OwnerCharacter = InCharacter;

	// 화살 포물선 구현
	const FVector StartLocation = GetActorLocation();
	const FVector TargetLocation = StartLocation + InForward * 500.0f;
	const float ProjectileSpeed = Projectile->InitialSpeed;

	FVector OutLaunchVelocity(0,0,0);
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this, OutLaunchVelocity, StartLocation, TargetLocation, ProjectileSpeed,
		false, 0, 150.0f);

	if(bHaveAimSolution)
	{
		FVector LaunchDirection = OutLaunchVelocity.GetSafeNormal();
		Projectile->Velocity = OutLaunchVelocity;

		// Arrow 액터 회전 설정
		FRotator NewRotation = LaunchDirection.Rotation();
		SetActorRotation(NewRotation);
	}

	else if (!bHaveAimSolution)
	{
		OutLaunchVelocity = InForward * ProjectileSpeed;
		Projectile->Velocity = OutLaunchVelocity;
	}

	Projectile->Activate();
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Particle Trail
	Particle->BeginTrails(FName("Trail1"), FName("Trail2"), 
		ETrailWidthMode_FromFirst, 1.0f);

	//CLog::Print(Projectile->Velocity);
}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Othercharacter = Cast<ACharacter>(OtherActor);

	CheckTrue(CHelpers::IsSameTeam(OwnerCharacter, Othercharacter));
	SetLifeSpan(LifeSpanAfterCollision);

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!!Othercharacter && OnHit.IsBound())
		OnHit.Broadcast(this, Othercharacter);

	// Arrow Trail Finish
	Particle->EndTrails();

	// Attach Arrow
	if (!!Othercharacter && !!Othercharacter->GetMesh())
	{
		UCStateComponent* hitted_State = CHelpers::GetComponent<UCStateComponent>(Othercharacter);
		// 가드 성공 시 Arrow 삭제
		if (hitted_State->IsGuardMode())
		{
			Destroy();
			return;
		}

		FName ClosestBoneName = Othercharacter->GetMesh()->FindClosestBone(Hit.ImpactPoint);

		// 맞았는지 체크
		if (!ClosestBoneName.IsNone())
		{
			//UE_LOG(LogTemp, Warning, TEXT("ClosestBoneName: %s"), *ClosestBoneName.ToString());

			FAttachmentTransformRules AttachRules
			(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
				true);

			AttachToComponent(Othercharacter->GetMesh(), AttachRules, ClosestBoneName);
			
			FTransform transform;
			transform.SetTranslation(Hit.ImpactPoint);
			CHelpers::PlayEffect(Othercharacter->GetWorld(), Niagara, transform);
		}
	}
}

void ACArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Othercharacter = Cast<ACharacter>(OtherActor);

	//CheckTrue(CHelpers::IsSameTeam(OwnerCharacter, Othercharacter));
	//SetLifeSpan(LifeSpanAfterCollision);

	//for (AActor* actor : Ignores)
	//	CheckTrue(actor == OtherActor);

	//Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//if (!!Othercharacter && OnHit.IsBound())
	//	OnHit.Broadcast(this, Othercharacter);

	//// Arrow Trail Finish
	//Particle->EndTrails();

	//// Attach Arrow
	//if (!!Othercharacter && !!Othercharacter->GetMesh())
	//{
	//	UCStateComponent* hitted_State = CHelpers::GetComponent<UCStateComponent>(Othercharacter);
	//	// 가드 성공 시 Arrow 삭제
	//	if (hitted_State->IsGuardMode())
	//	{
	//		Destroy();
	//		return;
	//	}

	//	FName ClosestBoneName = Othercharacter->GetMesh()->FindClosestBone(SweepResult.ImpactNormal);

	//	// 맞았는지 체크
	//	if (!ClosestBoneName.IsNone())
	//	{
	//		//UE_LOG(LogTemp, Warning, TEXT("ClosestBoneName: %s"), *ClosestBoneName.ToString());

	//		FAttachmentTransformRules AttachRules
	//		(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,
	//			true);

	//		AttachToComponent(Othercharacter->GetMesh(), AttachRules, ClosestBoneName);

	//		FTransform transform;
	//		transform.SetTranslation(SweepResult.ImpactPoint);
	//		CHelpers::PlayEffect(Othercharacter->GetWorld(), Niagara, transform);
	//	}
	//}

}

