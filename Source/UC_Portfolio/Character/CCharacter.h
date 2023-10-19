#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "CCharacter.generated.h"

UCLASS(Abstract)
class UC_PORTFOLIO_API ACCharacter
	: public ACharacter
	, public IICharacter
	, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeamID")
		uint8 TeamID;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE uint8 GetTeamID() { return TeamID; }

public:
	ACCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
