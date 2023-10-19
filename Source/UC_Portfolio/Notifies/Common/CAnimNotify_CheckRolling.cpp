#include "Notifies/Common/CAnimNotify_CheckRolling.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotify_CheckRolling::GetNotifyName_Implementation() const
{
	return "Rolling_True";
}

void UCAnimNotify_CheckRolling::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->SetRolling(CheckRolling);
}
