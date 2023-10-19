#include "Notifies/CAnimNotify_SpawnActor.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

FString UCAnimNotify_SpawnActor::GetNotifyName_Implementation() const
{
	return "SpawnActor";
}

void UCAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	// SpawnActor Location
	FVector loaction_Forward = OwnerCharacter->GetActorForwardVector() * Distance_Forward;
	FVector location_Up = OwnerCharacter->GetActorUpVector() * Distance_Up;
	FVector location_Right = OwnerCharacter->GetActorRightVector() * Distance_Right;

	FVector location = OwnerCharacter->GetActorLocation() + (loaction_Forward + location_Up + location_Right);

	// SpawnActor Rotation
	FRotator rotator = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetActorForwardVector());
	FRotator rotation = UKismetMathLibrary::MakeRotator((rotator.Roll) + Spawn_Rotator.Roll,(rotator.Pitch) + Spawn_Rotator.Pitch,(rotator.Yaw) + Spawn_Rotator.Yaw);

	// SpawnActor Scale
	FVector scale = Spawn_Scale;
	const FTransform transform = UKismetMathLibrary::MakeTransform(location, rotation, scale);
	
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;
	OwnerCharacter->GetWorld()->SpawnActor(ActorClass, &transform, params);
}
