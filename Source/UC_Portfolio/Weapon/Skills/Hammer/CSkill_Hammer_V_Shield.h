#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimeLineComponent.h"
#include "CSkill_Hammer_V_Shield.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACSkill_Hammer_V_Shield : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Shield_Sphere;
	
public:	
	ACSkill_Hammer_V_Shield();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void ShieldSmall(class ACharacter* InOwner);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ground_Timeline")
		class UCurveFloat* TimelineCurve;

	FTimeline Timeline;

private:
	FOnTimelineFloat TimeLineUpdateDelegate;
	FOnTimelineEvent TimeLineFinishDelegate;

private:
	UFUNCTION()
		void TimeLineUpdateFunc(float Output);

	UFUNCTION()
		void TimeLineFinishFunc();

private:
	UPROPERTY(EditAnywhere, Category = "Ground_Timeline")
		FVector LerpA = FVector(2.5f);

	UPROPERTY(EditAnywhere, Category = "Ground_Timeline")
		FVector LerpB = FVector::ZeroVector;

private:
	class ACharacter* Owner;

	UPROPERTY(EditAnywhere, Category = "Spawn_Scale")
		FVector Explosion_Scale = FVector(1.0f);

	UPROPERTY(EditAnywhere, Category = "Spawn_Class")
		TSubclassOf<class ACSkill_Hammer_V_Explosion> Explosion_Class;

	ACSkill_Hammer_V_Explosion* Explosion;

};
