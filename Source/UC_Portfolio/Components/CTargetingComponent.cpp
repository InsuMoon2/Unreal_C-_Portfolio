#include "Components/CTargetingComponent.h"

#include "CCamComponent.h"
#include "Global.h"
#include "Character/Enemies/CEnemy.h"
#include "Components/CMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Widgets/CUserWidget_CrossHair.h"

UCTargetingComponent::UCTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	MovementComponent = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	CheckNull(MovementComponent);

	CamComponent = CHelpers::GetComponent<UCCamComponent>(OwnerCharacter);
	CheckNull(CamComponent);

	StatusComponent = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	CheckNull(StatusComponent);


}

void UCTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!Target)
	{
		UCStateComponent* target_State;
		target_State = CHelpers::GetComponent<UCStateComponent>(Target);

		if (!target_State->IsDeadMode())
		{
			float distance;
			distance = OwnerCharacter->GetDistanceTo(Target);

			if (distance <= TraceDistance)
				Tick_Targeting();

			else
				End_Targeting();
		}
	}
}

void UCTargetingComponent::Toggle_Target()
{
	// TODO :: 타겟이 있으면 타겟팅을 끝내고, 없다면 타겟팅 시작
	if (!!Target)
	{
		End_Targeting();
		//CamComponent->DisableControlRotation();

		return;
	}

	Begin_Targeting();
	CamComponent->EnableControlRotation();
}

void UCTargetingComponent::Change_Focus(bool InRight)
{
	//CLog::Print(InRight);

	TMap<float, ACharacter*> Map;
	float Key = 0.0f;
	float Minimum = 999999.0f;
	ACharacter* Candidate = nullptr;

	CheckNull(Target);

	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = OwnerCharacter->GetActorLocation();

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	ignores.Add(Target);

	TArray<FHitResult> OutHits;
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = 5.0f;

	UKismetSystemLibrary::SphereTraceMultiByProfile(OwnerCharacter->GetWorld(),
		Start, End, TraceDistance, FName("Targeting"), false, ignores, DrawDebug,
		OutHits, true, TraceColor, TraceHitColor, DrawTime);

	//TArray<ACharacter*> Targets;
	for (FHitResult OutHit : OutHits)
	{
		FVector location;
		location = OutHit.GetActor()->GetActorLocation() - OwnerCharacter->GetActorLocation();
		FVector location_Unit = location.GetSafeNormal2D();

		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		FVector cross = FVector::CrossProduct(forward, location_Unit);

		float dot = FVector::DotProduct(cross, OwnerCharacter->GetActorUpVector());

		ACharacter* enemy = Cast<ACharacter>(OutHit.GetActor());

		Map.Add(dot, enemy);
	}

	for (const auto keys : Map)
	{
		float key = keys.Key;
		float abs_key = FMath::Abs(key);
		//ACharacter* value = keys.Value;

		// 오른쪽
		if (InRight == true)
		{
			if (key > 0.0f)
			{
				if (abs_key < Minimum)
				{
					Minimum = abs(key);

					Candidate = *Map.Find(key);
				}
			}

		}
		// 왼쪽
		else if (InRight == false)
		{
			if (key < 0.0f)
			{
				if (abs_key < Minimum)
				{
					Minimum = abs(key);

					Candidate = *Map.Find(key);
				}
			}
		}
	}

	// 같은팀이면 타겟팅 안되도록
	if (!CHelpers::IsSameTeam(OwnerCharacter, Candidate))
	{
		if (!!Candidate)
		{
			if (!bMovingFocus)
			{
				bMovingFocus = true;
				Change_Target(Candidate);
			}

			else
			{
				Change_Target(Candidate);
			}
		}

		else
			Change_Target(Candidate);

	}
	
}

void UCTargetingComponent::Right_Focus()
{
	Change_Focus(true);
}

void UCTargetingComponent::Left_Focus()
{
	Change_Focus(false);
}


void UCTargetingComponent::Begin_Targeting()
{
	// TODO :: 타겟팅 시작. MultiSphereTraceByProfile을 사용해서 범위 안에 들어오는 적들 타겟팅
	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = OwnerCharacter->GetActorLocation();

	TArray<AActor*> ignores;
	// TODO :: 나중에 팀이면 타겟팅 ignores에 추가시켜주기
	ignores.Add(OwnerCharacter);

	TArray<FHitResult> OutHits;
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = 5.0f;

	UKismetSystemLibrary::SphereTraceMultiByProfile(
		OwnerCharacter->GetWorld(), Start, End, 
		TraceDistance,FName("Targeting"), false, ignores, DrawDebug, OutHits, 
		true, TraceColor, TraceHitColor, DrawTime);

	TArray<ACharacter*> Targets;
	for (FHitResult OutHit : OutHits)
	{
		AActor* HitActor = OutHit.GetActor();

		ACharacter* HitCharacter = Cast<ACharacter>(HitActor);

		// HitActor와 OwnerCharacter의 클래스가 다르다면, 그리고 TeamID도 다를 때
		if (HitActor->GetClass() != OwnerCharacter->GetClass()//) 
			&& !CHelpers::IsSameTeam(HitCharacter, OwnerCharacter))
		{
			bTargetingCheck = true;

			// Targets에 Enemy 추가
			if (!!HitCharacter)
				Targets.Add(HitCharacter);
		}
	}

	if (bTargetingCheck)
	{
		//ACharacter* candidate;
		float angle = -2.0f;
		Target = CHelpers::GetNearlyFrontAngle(OwnerCharacter, Targets, angle);

		Change_Target(Target);
	}
	
}

void UCTargetingComponent::End_Targeting()
{
	// TODO :: Target을 비워주고, CrossHair가 있으면 꺼주고 DestroyComponent
	if (!!CrossHair)
	{
		Target = nullptr;
		CrossHair->SetVisibility(false);
		bTargetingCheck = false;
	}
}

void UCTargetingComponent::Change_Target(ACharacter* InCandidate)
{
	// TODO :: Candidate를 매개변수로 받아서 존재하지 않으면 EndTargeting 호출, 있으면 꺼주고 다시 타겟팅
	ACEnemy* enemy;
	enemy = Cast<ACEnemy>(InCandidate);

	if (!IsValid(InCandidate))
	{
		End_Targeting();

		return;
	}

	if (!!CrossHair)
	{
		CrossHair->SetVisibility(false);
	}
	
	CrossHair = enemy->GetCrossHair();
	CrossHair->SetVisibility(true);

	if (!CHelpers::IsSameTeam(OwnerCharacter, InCandidate))
	{
		Target = InCandidate;
	}
}

void UCTargetingComponent::Tick_Targeting()
{
	// TODO :: 타겟팅 적 보고있을 때 Control Rotation 보간처리 해주기
	FRotator ControlRotation;
	FRotator OwnerToTarget;

	ControlRotation = OwnerCharacter->GetControlRotation();

	FRotator CharacterToTarget;
	CharacterToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
	OwnerToTarget = FRotator(ControlRotation.Pitch, CharacterToTarget.Yaw, CharacterToTarget.Roll);
	
	bool Equal_Rotator;
	Equal_Rotator = UKismetMathLibrary::EqualEqual_RotatorRotator(ControlRotation, OwnerToTarget, FinishAngle);

	// 각이 똑같다면, 보간을 할 필요가 없다.
	if (Equal_Rotator)
	{
		OwnerCharacter->GetController()->SetControlRotation(OwnerToTarget);

		if (bMovingFocus)
			bMovingFocus = false;
	}

	else
	{
		FRotator controll;
		controll = UKismetMathLibrary::MakeRotator(OwnerToTarget.Roll, ControlRotation.Pitch, OwnerToTarget.Yaw);

		FRotator controll2;
		controll2 = UKismetMathLibrary::RInterpTo(ControlRotation, controll,
			UGameplayStatics::GetWorldDeltaSeconds(OwnerCharacter->GetWorld()), InterpSpeed);

		OwnerCharacter->GetController()->SetControlRotation(controll2);
	}
}


