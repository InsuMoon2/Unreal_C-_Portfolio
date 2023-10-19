#include "Weapon/Attachments/CAttachment_Wizard.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"

ACAttachment_Wizard::ACAttachment_Wizard()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "Wizard", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", SkeletalMesh);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/FantasyArmour/CharacterParts_Mage/Meshes/Weapons/SM_wpn_mage_staff_03.SM_wpn_mage_staff_03_SM_wpn_mage_staff_03_LOD0'");
	SkeletalMesh->SetSkeletalMesh(mesh);
}

void ACAttachment_Wizard::BeginPlay()
{
	Super::BeginPlay();
}

void ACAttachment_Wizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
