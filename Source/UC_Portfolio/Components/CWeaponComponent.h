#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/CWeaponStructures.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Bow, Sword, Hammer, Dagger, Wizard, GruxWeapon, UnArmed
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Skill1 = 0, Skill2, Skill3, Skill4,
	Skill_Upper, Skill_Execution, SKill_Parrying, Max, 
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS()
class UC_PORTFOLIO_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::UnArmed];

public:
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; }

	FORCEINLINE bool IsUnarmedMode() { return WeaponType == EWeaponType::UnArmed; }
	FORCEINLINE bool IsBowMode()	 { return WeaponType == EWeaponType::Bow; }
	FORCEINLINE bool IsSwordMode()	 { return WeaponType == EWeaponType::Sword; }
	FORCEINLINE bool IsHammerMode()	 { return WeaponType == EWeaponType::Hammer; }
	FORCEINLINE bool IsDaggerMode()  { return WeaponType == EWeaponType::Dagger; }
	FORCEINLINE bool IsWizardMode()  { return WeaponType == EWeaponType::Wizard; }
	FORCEINLINE bool IsGruxWeapon()  { return WeaponType == EWeaponType::GruxWeapon; }

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/*State, Weapon Component의 레벨이 같기 때문에 서로를 소유하면 안된다.
	그래서 둘 중에 하나가 서로를 소유하면 안된다. 참조만 해서 리턴받기 위해 만든 것이다*/
	bool IsIdleMode();
	bool IsActionMode();
	bool IsAirActionMode();
	bool IsExecutionMode();

public:
	class ACAttachment* GetAttachment();
	class UCEquipment*  GetEquipment();
	class UCDoAction*   GetDoAction();
	class UCDoAction*   GetAirAction();
	class UCSubAction*  GetSubAction();

	UFUNCTION(BlueprintPure)
		class UCSkill*		GetSkill(int32 InIndex);

public:
	// Setting Mode
	void SetUnarmedMode();
	void SetBowMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetDaggerMode();
	void SetWizardMode();
	void SetGruxWeaponMode();

public:
	void DoAction();

	void SubAction_Pressed();
	void SubAction_Released();

	void Skill1_Pressed();
	void Skill1_Released();

	void Skill2_Pressed();
	void Skill2_Released();

	void Skill3_Pressed();
	void Skill3_Released();

	void Skill4_Pressed();
	void Skill4_Released();

	// Sub Skill
	void Skill_AirAction();
	void Skill_Execution();
	void Skill_Parrying();

	float SetCoolTime(float InCoolTime, float InMaxCoolTime, float InDeltaSecond);

public:
	// PlugIn
	FORCEINLINE EWeaponType GetWeapon() { return WeaponType; }

	// WeaponType (Eum) -> String
	FString GetWeaponTypeAsString();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	FWeaponTypeChanged OnWeaponTypeChange;

	UPROPERTY(EditAnywhere, Category = "WeaponType")
		EWeaponType WeaponType = EWeaponType::UnArmed;

	UPROPERTY(EditAnywhere, Category = "SkillType")
		ESkillType Skill_Type = ESkillType::Max;

private:
	UPROPERTY()
		class UCWeaponData* Datas[(int32)EWeaponType::UnArmed];

private:
	class ACharacter* OwnerCharacter;

public:
	FORCEINLINE float GetCoolTime() { return CoolTime; }
	FORCEINLINE float GetMaxCoolTime() { return MaxCoolTime; }

private:
	float CoolTime;
	float MaxCoolTime;
};
