#include "Components/TargetComponent.h"
#include "Global.h"
#include "Components/CMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"

UTargetComponent::UTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}


void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTargetComponent::Begin_Targeting()
{

}

void UTargetComponent::End_Targeting()
{

}

void UTargetComponent::Change_Target()
{

}

void UTargetComponent::Tick_Targeting()
{

}

void UTargetComponent::Change_Focus()
{

}

