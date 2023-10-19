#include "Weapon/Skills/Grux/CSkill_Grux_Skill4_Decal.h"
#include "Components/DecalComponent.h"
#include "Global.h"

ACSkill_Grux_Skill4_Decal::ACSkill_Grux_Skill4_Decal()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "RootComponent");
	CHelpers::CreateComponent<UDecalComponent>(this, &Ground_Decal, "Decal", RootComponent);
}

void ACSkill_Grux_Skill4_Decal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACSkill_Grux_Skill4_Decal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSkill_Grux_Skill4_Decal::SetOwner(ACharacter* InOwner)
{
	OwnerCharacter = InOwner;
}

