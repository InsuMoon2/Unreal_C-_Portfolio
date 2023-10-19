#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength = 100.0f;

	UPROPERTY(EditAnywhere)
		FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
		bool bEnableCameraLag;

	UPROPERTY(EditAnywhere)
		FVector CameraLocation;
};

UCLASS(Blueprintable)
class UC_PORTFOLIO_API UCSubAction_Bow : public UCSubAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCUserWidget_CrossHair> CrossHairClass;

private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveVector* Curve;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData AimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		float AimingSpeed = 200.0f;

	// µð¹ö±ë
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundWave* BendingSound;

public:
	UCSubAction_Bow();

public:
	virtual void Pressed() override;
	virtual void Released() override;

public:
	void BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

public:
	void Tick_Implementation(float InDeltaTime) override;

private:
	UFUNCTION()
		void OnAiming(FVector Output);

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	FTimeline Timeline;

private:
	FAimData OriginData;

private:
	UPROPERTY()
		class UCUserWidget_CrossHair* CrossHair;

private:
	float* Bend;
	float Distance = 3000.0f;

};
