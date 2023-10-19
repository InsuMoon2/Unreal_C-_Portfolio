#pragma once

#include "CoreMinimal.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameplayDebuggerCategory.h"

class DEBUGGING_API FDebuggerCategory
	: public FGameplayDebuggerCategory
{
public:
	FDebuggerCategory();
	~FDebuggerCategory();

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;


private:
	struct FCategoryData
	{
		bool bDraw = false;
		FString Name;
		FVector Location;
		FVector Forward;

		class UCStateComponent* State;
		class UCWeaponComponent* Weapon;

		FString StateString;
		FString WeaponString;
	};

	FCategoryData PlayerPawnData;
	FCategoryData ForwardActorData;
	FCategoryData DebugActorData;

private:
	float TraceDistance = 1500.0f;
};
