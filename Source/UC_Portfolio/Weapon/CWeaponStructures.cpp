#include "Weapon/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CStatusComponent.h"
#include "Sound/SoundCue.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	UCCamComponent* camera_control = CHelpers::GetComponent<UCCamComponent>(InOwner);
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(InOwner);

	// ¸¶³ª
	CheckFalse(status->CompareInMana(ManaPoint));
	status->UseMana(ManaPoint);

	if (!!movement)
	{
		if (bFixedCamera)
			camera_control->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FDoActionData::PlayEffect(UWorld* InWorld, ACharacter* InCharacter, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InCharacter->GetActorLocation() + EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

void FDoActionData::PlayEffect(UWorld* InWorld, ACharacter* InCharacter, const FVector& InLocation, const FRotator& InRotation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InCharacter->GetActorLocation() + InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(InOther);

	if (ManaPoint > 0.0f)
		status->UseMana(ManaPoint);

	FActionDamageEvent damage;
	damage.HitData = this;

	InOther->TakeDamage(Power, damage, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));

	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;

			characters.Add(character);
		}
	}

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			for (ACharacter* character : characters)
				character->CustomTimeDilation = 1.0f;
		});

	InWorld->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void FHitData::PlayCameraShake(UWorld* InWorld, ACharacter* InAttacker)
{
	static bool bIsCameraShaking = false;
	FTimerHandle UnusedHandle;

	if (UnusedHandle.IsValid())
		InWorld->GetTimerManager().ClearTimer(UnusedHandle);

	if (!bIsCameraShaking)
	{
		//APlayerController* controller = InAttacker->GetController<APlayerController>();

		APlayerController* controller = UGameplayStatics::GetPlayerController(InWorld, 0);
		CheckNull(controller);
		controller->PlayerCameraManager->StartCameraShake(CameraShake);

		bIsCameraShaking = true;
	}

	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([&]()
		{
			bIsCameraShaking = false;
		});

	InWorld->GetTimerManager().SetTimer(UnusedHandle, TimerCallback, 0.3f, false);
}

void FHitData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}

bool FHitData::IsValidMontage()
{
	if (!!Montage)
		return true;

	return false;
}
