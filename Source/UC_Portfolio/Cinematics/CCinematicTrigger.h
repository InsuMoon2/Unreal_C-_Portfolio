#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCinematicTrigger.generated.h"

UCLASS(Blueprintable)
class UC_PORTFOLIO_API ACCinematicTrigger : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Cinematic_Trigger;

public:	
	ACCinematicTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnFinish_BossSequence();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character")
		class ACPlayer* Player;

};

