#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCamComponent.generated.h"

USTRUCT(BlueprintType)
struct FZoomData
{
	GENERATED_BODY()

	float Speed = 20.0f;
	float MinRange = 100.0f;
	float MaxRange = 600.0f;
	float InterpSpeed = 5.0f;
};

UCLASS()
class UC_PORTFOLIO_API UCCamComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "CameraSpeed")
		float HorizontalLook = 45;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "CameraSpeed")
		float VerticalLook = 45;

protected:
	virtual void BeginPlay() override;

public:
	// Camera
	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

	// Zoom
	//FORCEINLINE float GetZooming() { return ZoomData.MinRange = 250.0f; }
	

public:
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

	void EnableControlRotation();
	void DisableControlRotation();

private:
	// 캐릭터 세팅
	class ACPlayer* OwnerCharacter;

private:
	bool bFixedCamera;

private:
	UPROPERTY()
		float Zooming;

	FZoomData ZoomData;
};
