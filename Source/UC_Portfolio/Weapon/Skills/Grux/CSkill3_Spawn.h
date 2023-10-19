#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill3_Spawn.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API ACSkill3_Spawn : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = "Spawn")
		class UParticleSystemComponent* Particle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
		class USphereComponent* Sphere;

	
public:	
	ACSkill3_Spawn();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	class ACharacter* Hitted;

public:
	class ACharacter* OwnerCharacter;

public:
	UPROPERTY(EditAnywhere, Category = "Spawn_Decal")
		TSubclassOf<class ACSkill_Grux_Skill4_Decal> Decal;

private:
	UPROPERTY(EditAnywhere, Category = "Spawn_Decal")
		float decalZ = 20.0f;
};
