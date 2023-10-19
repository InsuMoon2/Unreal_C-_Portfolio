#pragma once

#include "CoreMinimal.h"
#include "CArrow.h"
#include "GameFramework/Actor.h"
#include "CFireBall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFireBallOverlap, class AActor*, InCauser, class ACharacter*, InOtherCharacter);

UCLASS(Blueprintable)
class UC_PORTFOLIO_API ACFireBall : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
		float LifeSpanAfterCollision = 5.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

public:
	FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

public:	
	ACFireBall();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void Shoot(const FVector& InForward);

private:
	UFUNCTION()
		void OnComponentBeginOverlap(
			UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor, UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, bool bFromSweep, 
			const FHitResult& SweepResult);

private:
	TArray<AActor*> Ignores;

public:
	FFireBallOverlap OnOverlap;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		ACharacter* OtherCharacter;

};

