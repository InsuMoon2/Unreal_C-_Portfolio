#include "Notifies/Execution/CAnimNotifyState_Execution.h"
#include "ActorSequenceComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "ActorSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"

FString UCAnimNotifyState_Execution::GetNotifyName_Implementation() const
{
	return "Execution_Camera";
}

void UCAnimNotifyState_Execution::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	// TODO :: 일단 Player로 실행해보고, Enemy 기준으로도 카메라 돌려보기.
	ACPlayer* Character = Cast<ACPlayer>(MeshComp->GetOwner());
	APlayerController* controller = UGameplayStatics::GetPlayerController(Character, 0);

	AActor* NewViewTarget = Character->IsSequence_Child();
	controller->SetViewTargetWithBlend(
		NewViewTarget, BlendInTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.0f);

	//UActorSequenceComponent* ActorSequence = Character->GetActorSequence();
	UActorSequenceComponent* ActorSequence = CHelpers::GetComponent<UActorSequenceComponent>(Character);

	if (!!ActorSequence)
	{
		UMovieSceneSequencePlayer* SequencePlayer = Cast<UMovieSceneSequencePlayer>(ActorSequence->GetSequencePlayer());
		SequencePlayer->Play();
	}
	
}

void UCAnimNotifyState_Execution::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	// TODO :: 일단 Player로 실행해보고, Enemy 기준으로도 카메라 돌려보기.
	ACPlayer* Character = Cast<ACPlayer>(MeshComp->GetOwner());
	APlayerController* controller = UGameplayStatics::GetPlayerController(Character, 0);

	AActor* NewViewTarget = Character->IsMain_Camera_Child();
	controller->SetViewTargetWithBlend(
		NewViewTarget, BlendOutTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.0f);
}
