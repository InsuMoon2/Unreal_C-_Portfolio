#include "Weapon/Attachments/CAttachmena_GruxWeapon.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACAttachmena_GruxWeapon::ACAttachmena_GruxWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	// Right
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Weapon_Right, "GruxWeapon_Right", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule_Right, "Capsule_Right", Weapon_Right);

	USkeletalMesh* right;
	CHelpers::GetAsset<USkeletalMesh>(&right, "SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshs3/Grux_Weapon_Solo.Grux_Weapon_Solo'");
	Weapon_Right->SetSkeletalMesh(right);

	// Left
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Weapon_Left, "GruxWeapon_Left", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule_Left, "Capsule_Left", Weapon_Left);

	USkeletalMesh* left;
	CHelpers::GetAsset<USkeletalMesh>(&left, "SkeletalMesh'/Game/ParagonGrux/Characters/Heroes/Grux/Meshs3/Grux_Weapon_Solo.Grux_Weapon_Solo'");
	Weapon_Left->SetSkeletalMesh(left);

}

void ACAttachmena_GruxWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ACAttachmena_GruxWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
