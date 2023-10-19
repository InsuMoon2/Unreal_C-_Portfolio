#include "Notifies/CAnimNotify_CameraShake.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraShake.h"
#include "Character/Enemies/CEnemy_Boss.h"

FString UCAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return "Camera_Shake";
}

void UCAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	APlayerController* player_Controller = character->GetController<APlayerController>();
	CheckNull(player_Controller);

	player_Controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);

}
