#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/CCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/AddOns/CGhostTrail.h"

// x가 true면 리턴
#define CheckTrue(x) { if((x) == true) return; }
// x가 true면 y 리턴
#define CheckTrueResult(x, y) { if((x) == true) return y; }

// x가 false면 리턴
#define CheckFalse(x) { if((x) == false) return;}
// x가 false면 y 리턴
#define CheckFalseResult(x, y) { if((x) == false) return y;}

// x가 nullptr이면 리턴
#define CheckNull(x) { if((x) == nullptr) return;}
// x가 nullptr이면 y 리턴
#define CheckNullResult(x, y) { if((x) == nullptr) return y;}


#define CreateTextRender()\
{\
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Tex", Root);\
	Text->SetRelativeLocation(FVector(0, 0, 100));\
	Text->SetRelativeRotation(FRotator(0, 180, 0));\
	Text->SetRelativeScale3D(FVector(2));\
	Text->TextRenderColor = FColor::Red;\
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;\
	Text->Text = FText::FromString(GetName().Replace(L"Default__", L""));\
}


class UC_PORTFOLIO_API CHelpers
{
public:
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr, FName InSocketName = NAME_None)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*OutComponent)->SetupAttachment(InParent, InSocketName);

			return;
		}

		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		*OutObject = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutClass = asset.Class;
	}

	template<typename T>
	static T* FindActor(UWorld* InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->IsA<T>())
				return Cast<T>(actor);
		}

		return nullptr;
	}

	template<typename T>
	static void FindActors(UWorld* InWorld, TArray<T*>& OutActors)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->IsA<T>())
				OutActors.Add(Cast<T>(actor));
		}
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static T* GetComponent(AActor* InActor, const FString& InName)
	{
		TArray<T*> components;
		InActor->GetComponents<T>(components);

		for (T* component : components)
		{
			if (component->GetName() == InName)
				return component;
		}

		return nullptr;
	}

	static void AttachTo(AActor* InActor, USceneComponent* InParent, FName InSocketName)
	{
		InActor->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
	}

	static void PlayEffect(UWorld* InWorld, UFXSystemAsset* InAsset, const FTransform& InTransform, USkeletalMeshComponent* InMesh = nullptr,
		FName InSocketName = NAME_None)
	{
		UParticleSystem* particle = Cast<UParticleSystem>(InAsset);
		UNiagaraSystem* niagara = Cast<UNiagaraSystem>(InAsset);

		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

		if (!!InMesh)
		{
			if (!!particle)
			{
				UGameplayStatics::SpawnEmitterAttached(particle, InMesh, InSocketName, location, rotation, scale);

				return;
			}

			if (!!niagara)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, InMesh, InSocketName, location, rotation, scale,
					EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);

				return;
			}
		}

		if (!!particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, InTransform);

			return;
		}

		if (!!niagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotation, scale);

			return;
		}
	}

	static ACGhostTrail* Play_GhostTrail(TSubclassOf<ACGhostTrail>& InClass, class ACharacter* InOwner)
	{
		CheckNullResult(InClass, nullptr);
		CheckNullResult(InOwner, nullptr);

		FActorSpawnParameters params;
		params.Owner = InOwner;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector location = InOwner->GetActorLocation();
		location.Z -= InOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FTransform transform;
		transform.SetTranslation(location);

		return InOwner->GetWorld()->SpawnActor<ACGhostTrail>(InClass, transform, params);
	}

	static ACharacter* GetNearlyFrontAngle(class ACharacter* InCharacter, TArray<ACharacter*>InArray, float angle)
	{
		//float angle = - 2.0f;
		ACharacter* candidate = nullptr;

		for (ACharacter* hitted : InArray)
		{
			if (!IsSameTeam(hitted, InCharacter))
			{
				FVector direction = hitted->GetActorLocation() - InCharacter->GetActorLocation();
				direction = direction.GetSafeNormal2D();

				FVector forward = FQuat(InCharacter->GetActorRotation()).GetForwardVector();

				float dot = FVector::DotProduct(direction, forward);

				if (dot >= angle)
				{
					angle = dot;
					candidate = hitted;
				}

			}

		}

		return candidate;
	}

	static bool DotForward(AActor* InActor, ACharacter* InOther)
	{
		// TODO :: 내적해서 Attacker와 위치 구해서 앞뒤 판별하기
		FVector attacker_location = InActor->GetActorLocation();
		FVector other_location = InOther->GetActorLocation();

		FVector unit = attacker_location - other_location;
		unit = unit.GetSafeNormal2D();

		float dot = FVector::DotProduct(unit, InOther->GetActorForwardVector());

		if (dot >= 0.002)
			return true;

		return false;
	}

	static bool IsSameTeam(ACharacter* InOwner, ACharacter* InEnemy)
	{
		CheckNullResult(InOwner, false);
		CheckNullResult(InEnemy, false);

		ACCharacter* Owner = Cast<ACCharacter>(InOwner);
		ACCharacter* Enemy = Cast<ACCharacter>(InEnemy);

		int32 OwnerID = Owner->GetTeamID();
		int32 EnemyID = Enemy->GetTeamID();

		if (OwnerID == EnemyID)
		{
			return true;
		}

		return false;
	}

};