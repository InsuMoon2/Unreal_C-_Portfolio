#include "Weapon/Attachments/CAttachment_Sword.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACAttachment_Sword::ACAttachment_Sword()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "Sword", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", SkeletalMesh);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Weapons/DarkSword/SM_SwordOfDarkness_SM_SwordOfDarkness.SM_SwordOfDarkness_SM_SwordOfDarkness'");
	SkeletalMesh->SetSkeletalMesh(mesh);

	CHelpers::GetAsset<UMaterialInterface>(&FireMaterial, "Material'/Game/Sword_Fx/Materials/M_Sword04.M_Sword04'");
	CHelpers::GetAsset<UMaterialInterface>(&OriginMaterial, "MaterialInstanceConstant'/Game/Assets/CloseCombat/Weapon/Swords/SwordOfDarkness/Materials/MI_SwordOfDarkness.MI_SwordOfDarkness'");

	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", SkeletalMesh);
	UNiagaraSystem* niagara;
	CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/Sword_Fx/Effects/NS_Sword_FX04.NS_Sword_FX04'");
	Niagara->SetAsset(niagara);
	Niagara->SetVisibility(false);
}

void ACAttachment_Sword::BeginPlay()
{
	Super::BeginPlay();

	//ChangeMaterial();
}

void ACAttachment_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAttachment_Sword::ChangeFireMaterial()
{
	SkeletalMesh->SetMaterial(0, FireMaterial);
}

void ACAttachment_Sword::ChangeMaterialReset()
{
	SkeletalMesh->SetMaterial(0, OriginMaterial);
}



