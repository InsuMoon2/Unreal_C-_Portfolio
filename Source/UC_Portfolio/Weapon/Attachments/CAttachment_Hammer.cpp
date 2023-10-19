#include "Weapon/Attachments/CAttachment_Hammer.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"

ACAttachment_Hammer::ACAttachment_Hammer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "Hammer", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", SkeletalMesh);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/Hammer/Hammer.Hammer'");
	SkeletalMesh->SetSkeletalMesh(mesh);
}

void ACAttachment_Hammer::BeginPlay()
{
	Super::BeginPlay();
}

void ACAttachment_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
