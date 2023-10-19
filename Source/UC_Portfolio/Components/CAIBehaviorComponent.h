#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid, Dead, Max,
};

UENUM(BlueprintType)
enum class EAISkillType : uint8
{
	Skill1 = 0, Skill2, Skill3, Skill4, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_PORTFOLIO_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName PatrolLocationKey = "Patrol_Location";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName AvoidLocationKey = "Avoid_Location";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName SkillTimeKey = "Skill_Time";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName AISkillTypeKey = "Skill_Type";

	UPROPERTY(EditAnywhere, Category = "Key")
		FName DistanceKey = "Distance";

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	EAIStateType GetAIStateType();
	ACharacter* GetTarget();
	void SetTarget(class ACharacter* InCharacter);

	EAISkillType GetAISkillType();
	float GetDistance();

public:
	FVector GetPatrolLoaction();
	void SetPatrolLocation(const FVector& InLocation);

	FVector GetAvoidLocation();

	float GetSkillTime();
	void SetSkillTime(const float InDeltaTime);

public:
	// State
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsHittedMode();
	bool IsAvoidMode();
	bool IsDeadMode();

public:
	// Skill
	bool IsSkill1();
	bool IsSkill2();
	bool IsSkill3();
	bool IsSkill4();

public:
	UCAIBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:
	// State
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();

	// Skill
	void SetSkill_Type(EAISkillType InSkillType);

	void SetDistance(float InDistance);

private:
	void ChangeType(EAIStateType InType);

public:
	FAIStateTypeChanged OnAIStateTypeChanged;

private:
	class UBlackboardComponent* Blackboard;

	UPROPERTY(EditAnywhere, Category = "SkillType")
		EAISkillType SkillType = EAISkillType::Skill1;
};
