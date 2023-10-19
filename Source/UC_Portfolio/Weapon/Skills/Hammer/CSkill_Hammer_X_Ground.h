#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Hammer_X_Ground.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACSkill_Hammer_X_Ground : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData BossHitData;

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* Particle;

public:
	ACSkill_Hammer_X_Ground();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnParticleFinished(UParticleSystemComponent* FinishedComponent);

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	class ACharacter* Hitted;

};
