#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/CWeaponStructures.h"
#include "CRotate_Object.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API ACRotate_Object : public AActor
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
        FHitData HitData;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
        float Speed = 300;

    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
        float Distance = 150;

    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
        bool bNegative;

    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
        float DamageInteval = 0.1f;

private:
    UPROPERTY(VisibleDefaultsOnly)
        class UCapsuleComponent* Capsule;

    UPROPERTY(VisibleDefaultsOnly)
        class UParticleSystemComponent* Particle;
    // TODO :: 나이아가라로 바꿀 수도 있으니까 만들어 논거임
    UPROPERTY(VisibleDefaultsOnly)
        class UNiagaraComponent* Niagara;

public:	
	ACRotate_Object();

protected:
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
        void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
        void SendDamage();


private:
    float Angle;

    TArray<ACharacter*> Hitted;
    FTimerHandle TimerHandle;

};
