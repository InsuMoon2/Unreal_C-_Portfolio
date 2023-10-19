#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimeLineComponent.h"
#include "Weapon/CWeaponStructures.h"
#include "CSkill_Hammer_V_Ground.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API ACSkill_Hammer_V_Ground : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere, Category = "Distance")
		class UAnimMontage* DrawOut_Montage;

	UPROPERTY(EditAnywhere, Category = "Distance")
		class UAnimMontage* Boss_DrawOut_Montage;

public:
	ACSkill_Hammer_V_Ground();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void GroundSmall(class ACharacter* InOwner);

protected:
	// Ground
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ground_Timeline")
		class UCurveFloat* Ground_TimelineCurve;

	FTimeline Ground_Timeline;

	// Draw
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ground_Timeline")
		class UCurveFloat* Draw_TimelineCurve;

	FTimeline  Draw_Timeline;

private:
	FOnTimelineFloat Ground_TimeLineUpdateDelegate;
	FOnTimelineEvent Ground_TimeLineFinishDelegate;

	FOnTimelineFloat Draw_TimeLineUpdateDelegate;
	FOnTimelineEvent Draw_TimeLineFinishDelegate;

private:
	UFUNCTION()
		void Ground_TimeLineUpdateFunc(float Output);

	UFUNCTION()
		void Ground_TimeLineFinishFunc();

	UFUNCTION()
		void Draw_TimeLineUpdateFunc(float Output);

	UFUNCTION()
		void Draw_TimeLineFinishFunc();

private:
	UPROPERTY(EditAnywhere, Category = "Ground_Timeline")
		FVector LerpA = FVector(2.5f);

	UPROPERTY(EditAnywhere, Category = "Ground_Timeline")
		FVector LerpB = FVector::ZeroVector;

	ACharacter* Owner;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;

	UPROPERTY(EditAnywhere, Category = "Distance")
		float LerpDistance = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Distance")
		float LerpTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Distance")
		float DrawTime = 2.0f;

	TArray<class ACharacter*> Hitted;

	FTimerHandle Ground_TimerHandle;
	FTimerHandle Draw_TimerHandle;

public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};