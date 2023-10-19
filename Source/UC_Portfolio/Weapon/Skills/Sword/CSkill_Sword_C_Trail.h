#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Sword_C_Trail.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACSkill_Sword_C_Trail
	: public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData BossHitData;

	// 충돌될 때 연속적으로 데미지가 들어가는 간격이다.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DamageInterval = 0.1f;

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* Fire;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* Fire_Ground;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* Projectile;
	
public:	
	ACSkill_Sword_C_Trail();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnParticleFinished(UParticleSystemComponent* FinishedComponent);

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	TArray<class ACharacter*> Hitted;
	FTimerHandle TimerHandle;

};
