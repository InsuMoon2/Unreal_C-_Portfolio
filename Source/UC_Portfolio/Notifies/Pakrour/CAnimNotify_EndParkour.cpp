#include "Notifies/Pakrour/CAnimNotify_EndParkour.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Parkour/CParkourComponent.h"

FString UCAnimNotify_EndParkour::GetNotifyName_Implementation() const
{
	return "Parkour";
}

void UCAnimNotify_EndParkour::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(MeshComp->GetOwner());
	CheckNull(parkour);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);

	state->SetIdleMode();
	parkour->End_DoParkour();
}
