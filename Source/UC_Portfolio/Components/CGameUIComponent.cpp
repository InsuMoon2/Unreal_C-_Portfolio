#include "Components/CGameUIComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/CWeaponAsset.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CSkill.h"
#include "Weapon/CSubAction.h"

UCGameUIComponent::UCGameUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCGameUIComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void UCGameUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

