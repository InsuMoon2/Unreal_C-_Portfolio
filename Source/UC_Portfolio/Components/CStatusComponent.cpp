#include "Components/CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	Health = MaxHealth;
	Mana = MaxMana;
	Incapacitation = MaxIncapacitation;
}

void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCStatusComponent::Damage(float InAmount)
{
	Health += (InAmount * -1.0f);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	Incapacitation += (InAmount * -1.0f);
	Incapacitation = FMath::Clamp(Incapacitation, 0.0f, MaxIncapacitation);
}

void UCStatusComponent::UseMana(float InMana)
{
	CheckFalse(Mana > InMana);

	Mana += (InMana * -1.0f);
}

void UCStatusComponent::ChargeMana()
{
	Mana += AddMana;
	Mana = FMath::Clamp(Mana, 0.0f, MaxMana);

	//CLog::Print(Mana);
}

