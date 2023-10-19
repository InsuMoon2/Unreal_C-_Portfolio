#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCWeaponData : public UObject
{
	GENERATED_BODY()

private:
	friend class UCWeaponAsset;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class UCSubAction* GetSubAction() { return SubAction; }
	FORCEINLINE class UCDoAction* GetAirAction() { return AirAction; }
	FORCEINLINE class UCSkill* GetSkill(int32 Skill_Index) { return Skills[Skill_Index]; }

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
};
