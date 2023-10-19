#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0, Roll, BackStep, Jump, Equip, Hitted, Dead, Action, AirAction,
	Parkour, Incapacitation, Execution, Guard, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class UC_PORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }
	FORCEINLINE bool IsBackstepMode() { return Type == EStateType::BackStep; }
	FORCEINLINE bool IsJumpMode() { return Type == EStateType::Jump; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsParkourMode() { return Type == EStateType::Parkour; }

	FORCEINLINE bool IsIncapacitationMode() { return Type == EStateType::Incapacitation; }
	FORCEINLINE bool IsExecutionMode() { return Type == EStateType::Execution; }
	FORCEINLINE bool IsGuardMode() { return Type == EStateType::Guard; }

	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }
	FORCEINLINE bool IsSkillMode()	   { return bInSkillMode; }
	FORCEINLINE bool IsAirActionMode() { return bInAirActionMode; }
	FORCEINLINE bool IsExecution() { return bExecution; }
	FORCEINLINE bool IsParryingMode() { return bParrying; }

public:
	// PlugIn
	FORCEINLINE EStateType GetState() {return Type;}
	FString GetStateTypeAsString();

public:
	void SetIdleMode();
	void SetRollMode();
	void SetBackStepMode();
	void SetJumpMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetParkourMode();
	void SetIncapacitationMode();
	void SetExecutionMode();
	void SetGuardMode();

	void OnSubActionMode();
	void OffSubActionMode();

	void OnSkillMode();
	void OffSkillMode();

	void OnAirActionMode();
	void OffAirActionMode();

	void OnExecutionMode();
	void OffExecutionMode();

	void OnParryingMode();
	void OffParryingMode();

private:
	void ChangeType(EStateType InType);

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	class ACharacter* OwnerCharacter;
	bool bJump = true;

	EStateType Type;

private:
	bool bInSubActionMode;
	bool bInSkillMode;
	bool bInAirActionMode;
	bool bExecution;
	bool bParrying;
};
