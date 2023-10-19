#include "Components/CRidingComponent.h"

UCRidingComponent::UCRidingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCRidingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCRidingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

