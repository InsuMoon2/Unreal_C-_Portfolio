#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapon/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

const int32 Skill_MAX = 7;

UCLASS()
class UC_PORTFOLIO_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> Attachment_Class;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> Equipment_Class;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction> DoAction_Class;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSubAction> SubAction_Class;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction> AirAction_Class;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class UCSkill>> Skill_Class;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Hit Datas")
		TArray<FHitData> GuardHitDatas;

	UPROPERTY(EditAnywhere, Category = "Hit Datas")
		TArray<FHitData> Boss_HitDatas;

	UPROPERTY(EditAnywhere, Category = "AirAction")
		TArray<FDoActionData> Air_ActionDatas;

	UPROPERTY(EditAnywhere, Category = "AirAction")
		TArray<FHitData> Air_HitDatas;

	UPROPERTY(EditAnywhere, Category = "AirAction")
		TArray<FHitData> Boss_Air_HitDatas;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment*  GetEquipment()  { return Equipment;  }
	FORCEINLINE class UCDoAction*   GetDoAction()   { return DoAction;   }

	FORCEINLINE class UCSubAction*  GetSubAction()  { return SubAction;  }
	FORCEINLINE class UCDoAction*   GetAirAction()  { return AirAction;  }
	FORCEINLINE class UCSkill*      GetSkill(int32 Skill_Index) { return Skills[Skill_Index]; }

private:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoAction* DoAction;

	UPROPERTY()
		class UCSubAction* SubAction;

	UPROPERTY()
		class UCDoAction* AirAction;

	UPROPERTY()
		TArray<class UCSkill*> Skills;

// 플러그인
#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;

#endif // WITH_EDITOR
};
