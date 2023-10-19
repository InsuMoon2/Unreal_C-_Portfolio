#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Components/CStateComponent.h"
#include "CMontagesComponent.generated.h"

USTRUCT()
struct FMontagesData
	: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;
};

UCLASS()
class UC_PORTFOLIO_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* DataTable;

public:	
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

private:
	bool IsIncapacitation();

public:
	void PlayDoubleJump();
	void PlayBackStepMode();
	void PlayRollMode();
	void PlayIncapacitation();
	void PlayDeadMode();

	void PlayParryingHitted();

private:
	void PlayAnimMontage(EStateType InType);
	void PlayAnimMontage(EStateType InType, UInputComponent* Input);

private:
	class ACharacter* OwnerCharacter;

private:
	FMontagesData* Datas[(int32)EStateType::Max];

private:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* ParryingHitted;
};
