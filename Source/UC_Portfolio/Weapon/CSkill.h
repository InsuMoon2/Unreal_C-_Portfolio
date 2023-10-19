#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon/CWeaponStructures.h"
#include "Components/CWeaponComponent.h"
#include "CSkill.generated.h"

UCLASS(Abstract)
class UC_PORTFOLIO_API UCSkill : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetCoolTime() { return Skill_CoolTime; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetMaxCoolTime() { return Skill_MaxCoolTime; }
public:
	UCSkill();

	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCDoAction* InDoAction
	);

public:
	// Skill
	virtual void Skill_Pressed(ESkillType InSkillIndex) {};
	virtual void Skill_Released(ESkillType InSkillIndex) {};

	virtual void Skill_Upper() {}
	virtual void Skill_Execution() {}
	virtual void Skill_Parrying() {}

public:
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Skill();
	virtual void Begin_Skill_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void End_Skill();
	virtual void End_Skill_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}

protected:
	// 가지고 있을 객체
	class ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	// Component
	class UCWeaponComponent* Weapon;
	class UCStateComponent* State;
	class UCMovementComponent* Movement;
	class UCCamComponent* Camera;
	class UCStatusComponent* Status;
	class UCInGameUIComponent* GameUI;

	bool bInAction;

	float Skill_CoolTime = 0.0f;
	float Skill_MaxCoolTime;
};
