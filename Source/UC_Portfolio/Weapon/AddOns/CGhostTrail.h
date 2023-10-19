#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACGhostTrail : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float StartDelay = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float Interval = 0.25;

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		FLinearColor Color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 거리 밀도
	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		float Exponent = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		FVector Scale = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly, Category = "Capture")
		FVector ScaleAmount = FVector::ZeroVector;

protected:
	// 캡처해야 하기 때문에 PosableMeshCompnent를 만들어주었다.
	UPROPERTY(BlueprintReadWrite)
		class UPoseableMeshComponent* Mesh;

public:	
	ACGhostTrail();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void PlayGhostTrail();
	void EndGhostTrail();

	void SubGhostTrail_Alpha();

private:
	class ACharacter* Owner;
	class UMaterialInstanceDynamic* Material;

	FTimerHandle TimerHandle;

private:

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
		class UCurveFloat* GhostTrail_Curve;

	UPROPERTY(VisibleAnywhere, Category = "GhostTrail")
		class UTimelineComponent* Timeline;
};
