#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon/CWeaponStructures.h"
#include "CDoAction.generated.h"

// 객체화가 되면 안되기 때문에 Abstract로 추상 클래스로 만들어 주었음.
UCLASS(Abstract)
class UC_PORTFOLIO_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }

	FORCEINLINE int32 GetIndex() { return Index; }


public:
	UCDoAction();

	virtual void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas,
		const TArray<FHitData>& InGuardHitDatas,
		const TArray<FHitData>& InBossHitDatas,
		const TArray<FHitData>& InBossAirHitDatas
	);

	virtual void Tick(float InDeltaTime) {}

public:
	// 직접 호출은 하지 않을 것이다. 그래서 모드 변경만 해줄 것임.
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

	//virtual void Change_Focus(FRotator InTarget, AController* Incontroller) {};

public:
	UFUNCTION()
		virtual void OnBeginEquip() { }

	UFUNCTION()
		virtual void OnUnequip() { }

public:
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() { }

	UFUNCTION()
		virtual void OnAttachmentEndCollision() { }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) { }

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) { }

protected:
	bool bInAction;
	bool bBeginAction;

	class ACharacter* OwnerCharacter;
	class ACAttachment* Attachment;
	class UWorld* World;

	class UCWeaponComponent* Weapon;
	class UCStateComponent* State;
	class UCMovementComponent* Movement;
	class UCCamComponent* CameraControl;
	class UCMontagesComponent* Montage;

	TArray<FDoActionData> DoActionDatas;

	TArray<FHitData> HitDatas;
	TArray<FHitData> GuardHitDatas;
	TArray<FHitData> BossHitDatas;
	TArray<FHitData> Boss_AirHitDatas;

protected:
	int32 Index = 0;
};
