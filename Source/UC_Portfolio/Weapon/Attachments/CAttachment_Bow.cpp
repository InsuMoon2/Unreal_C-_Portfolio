#include "Weapon/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Character/Enemies/CEnemy.h"
#include "Weapon/AnimInstances/CAnimInstance_Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/StaticMeshComponent.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &QuiverMesh, "QuiverMesh", Root);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "AnimBlueprint'/Game/_Game/Weapon/Bow/ABP_ElvenBow.ABP_ElvenBow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);

}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");
	AttachToMesh("spine_Quiver", QuiverMesh);
	QuiverMesh->SetVisibility(false);
	SkeletalMesh->SetVisibility(false);

	ACharacter* character = Cast<ACEnemy>(OwnerCharacter);
	if (!!character)
	{
		QuiverMesh->SetVisibility(true);
		SkeletalMesh->SetVisibility(true);

		return;
	}

		
	// 앞의 SkeletaMesh는 Component이다.
	//PoseableMesh->SetSkeletalMesh(SkeletalMesh->SkeletalMesh);
	//PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Bow_Left");
	SkeletalMesh->SetVisibility(true);
	QuiverMesh->SetVisibility(true);

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		OriginViewPitchRange.X = controller->PlayerCameraManager->ViewPitchMin;
		OriginViewPitchRange.Y = controller->PlayerCameraManager->ViewPitchMax;

		controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;
	}
}

void ACAttachment_Bow::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	AttachTo("Holster_Bow");
	SkeletalMesh->SetVisibility(false);
	QuiverMesh->SetVisibility(false);

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
	}
}

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();
}



