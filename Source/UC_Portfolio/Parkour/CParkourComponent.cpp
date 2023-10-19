#include "Parkour/CParkourComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CStateComponent.h"

//#define LOG_UCParkourComponent 

void FParkourData::PlayMontage(class ACharacter* InCharacter)
{
	if (bFixedCamera)
	{
		UCCamComponent* Cam = CHelpers::GetComponent<UCCamComponent>(InCharacter);

		if (!!Cam)
			Cam->EnableFixedCamera();
	}

	InCharacter->PlayAnimMontage(Montage, PlayRatio, SectionName);
}

// 구조체 함수
///////////////////////////////////////////////////////////////////////////////

UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/_Game/Datas/DT_Parkour.DT_Parkour'");
}

void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FParkourData*> datas;
	DataTable->GetAllRows<FParkourData>("", datas);

	for (int32 i = 0; i < (int32)EParkourType::Max; i++)
	{
		TArray<FParkourData> temp;

		for (FParkourData* data : datas)
		{
			if (data->Type == (EParkourType)i)
				temp.Add(*data);
		}

		DataMap.Add((EParkourType)i, temp);
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	USceneComponent* arrow = CHelpers::GetComponent<USceneComponent>(OwnerCharacter, "ArrowGroup");

	TArray<USceneComponent*> components;
	arrow->GetChildrenComponents(false, components);

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
		Arrows[i] = Cast<UArrowComponent>(components[i]);
}

void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	

	CheckTrace_Land();
}

void UCParkourComponent::LineTrace(EParkourArrowType InType)
{
	UArrowComponent* arrow = Arrows[(int32)InType];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * TraceDistance;


	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery3,
		false, ignores, DebugType, HitResults[(int32)InType], true, color,
		FLinearColor::White);
}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::Center;
	LineTrace(type);

	const FHitResult& hitResult = HitResults[(int32)type];
	CheckFalse(hitResult.bBlockingHit);

	UStaticMeshComponent* mesh = CHelpers::GetComponent<UStaticMeshComponent>(hitResult.GetActor());
	CheckNull(mesh);

	HitObstacle = hitResult.GetActor();

	FVector minBound, maxBound;
	mesh->GetLocalBounds(minBound, maxBound);

	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);
	HitObstacleExtent = FVector(x, y, z);

	HitDistance = hitResult.Distance;
	// ImpactNormal -> 충돌체의 노말벡터
	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;

#ifdef LOG_UCParkourComponent
	CLog::Print(HitObstacle, 10);
	CLog::Print(HitObstacleExtent, 11);
	CLog::Print(HitDistance, 12);
	CLog::Print(ToFrontYaw, 13);
#endif //LOG_UCParkourComponent
}

void UCParkourComponent::CheckTrace_Ceil()
{
	LineTrace(EParkourArrowType::Ceil);
}

void UCParkourComponent::CheckTrace_Floor()
{
	LineTrace(EParkourArrowType::Floor);
}

void UCParkourComponent::CheckTrace_LeftRight()
{
	LineTrace(EParkourArrowType::Left);
	LineTrace(EParkourArrowType::Right);
}

void UCParkourComponent::CheckTrace_Land()
{
	CheckFalse(OwnerCharacter->GetCharacterMovement()->IsFalling());

	CheckTrue(bFalling);
	bFalling = true;

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Land];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();
	FVector start = transform.GetLocation();

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);
	FVector end = start + transform.GetRotation().GetForwardVector() * (*datas)[0].Extent;

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DebugType, HitResults[(int32)EParkourArrowType::Land], true, color, FLinearColor::White);
}

bool UCParkourComponent::Check_Obstacle()
{
	CheckNullResult(HitObstacle, false);

	bool b = true;
	b &= HitResults[(int32)EParkourArrowType::Center].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Left].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Right].bBlockingHit;
	CheckFalseResult(b, false);

	FVector center = HitResults[(int32)EParkourArrowType::Center].Normal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].Normal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].Normal;

	CheckFalseResult(center.Equals(left), false);
	CheckFalseResult(center.Equals(right), false);

	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;
	FVector end = OwnerCharacter->GetActorLocation();
	float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;

	FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
	float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;

	float yaw = abs(abs(lookAt) - abs(impactAt));

	CheckFalseResult(yaw <= AvailableFrontAngle, false);

	return true;
}

void UCParkourComponent::DoParkour(bool bLanded)
{
	CheckFalse(Type == EParkourType::Max);

	if (bLanded && Check_FallMode())
	{
		DoParkour_Fall();

		return;
	}

	HitObstacle = NULL;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0;
	ToFrontYaw = 0;

	CheckTrace_Center();

	if (!!HitObstacle)
	{
		CheckTrace_Ceil();
		CheckTrace_Floor();
		CheckTrace_LeftRight();
	}

	CheckFalse(Check_Obstacle());
	if (Check_ClimbMode())
	{
		DoParkour_Climb();

		return;
	}

	if (Check_SlideMode())
	{
		DoParkour_Slide();

		return;
	}

	FParkourData data;
	if (Check_ObstacleMode(EParkourType::Normal, data))
	{
		DoParkour_Obstacle(EParkourType::Normal, data);

		return;
	}

	if (Check_ObstacleMode(EParkourType::Short, data))
	{
		DoParkour_Obstacle(EParkourType::Short, data);

		return;
	}

	if (Check_ObstacleMode(EParkourType::Wall, data))
	{
		DoParkour_Obstacle(EParkourType::Wall, data);

		return;
	}
}

void UCParkourComponent::End_DoParkour()
{
	switch (Type)
	{
	case EParkourType::Climb:
		End_DoParkour_Climb();
		break;

	case EParkourType::Fall:
		End_DoParkour_Fall();
		break;

	case EParkourType::Slide:
		End_DoParkour_Slide();
		break;

	case EParkourType::Short:
	case EParkourType::Normal:
	case EParkourType::Wall:
		End_DoParkour_Obstacle();
		break;
	}

	Type = EParkourType::Max;

	UCCamComponent* Cam = CHelpers::GetComponent<UCCamComponent>(OwnerCharacter);

	if (!!Cam)
		Cam->DisableFixedCamera();
}

bool UCParkourComponent::Check_ClimbMode()
{
	CheckFalseResult(HitResults[(int32)EParkourArrowType::Ceil].bBlockingHit, false);

	// Find는 값을 리턴하는 것이 아니라 포인터를 리턴한다.
	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Climb);
	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);
	CheckFalseResult(FMath::IsNearlyEqual((*datas)[0].Extent, HitObstacleExtent.Z, 10), false);

	return true;
}

void UCParkourComponent::DoParkour_Climb()
{
	Type = EParkourType::Climb;

	OwnerCharacter->SetActorLocation(HitResults[(int32)EParkourArrowType::Center].ImpactPoint);
	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	(*DataMap.Find(EParkourType::Climb))[0].PlayMontage(OwnerCharacter);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UCParkourComponent::End_DoParkour_Climb()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UCParkourComponent::Check_FallMode()
{
	CheckFalseResult(bFalling, false);
	bFalling = false;

	float distance = HitResults[(int32)EParkourArrowType::Land].Distance;

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);
	CheckFalseResult((*datas)[0].MinDistance < distance, false);
	CheckFalseResult((*datas)[0].MaxDistance > distance, false);

	return true;
}

void UCParkourComponent::DoParkour_Fall()
{
	Type = EParkourType::Fall;

	(*DataMap.Find(EParkourType::Fall))[0].PlayMontage(OwnerCharacter);
}

void UCParkourComponent::End_DoParkour_Fall()
{
	// TODO :: 딱히 할게 없나?
}

bool UCParkourComponent::Check_SlideMode()
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Floor].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Slide);
	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);


	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Floor];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector arrowLocation = transform.GetLocation();
	FVector ownerLocation = OwnerCharacter->GetActorLocation();

	float extent = (*datas)[0].Extent;
	float z = arrowLocation.Z + extent;

	FVector forward = OwnerCharacter->GetActorForwardVector();
	forward = FVector(forward.X, forward.Y, 0);

	FVector start = FVector(arrowLocation.X, arrowLocation.Y, z);
	FVector end = start + forward * TraceDistance;

	TArray<AActor*> ignores;
	FHitResult hitResult;

	UKismetSystemLibrary::BoxTraceSingle(GetWorld(), start, end, FVector(0, extent, extent),
		OwnerCharacter->GetActorRotation(), ETraceTypeQuery::TraceTypeQuery1, false,
		ignores, DebugType, hitResult, true);

	CheckTrueResult(hitResult.bBlockingHit, false);

	return true;
}


void UCParkourComponent::DoParkour_Slide()
{
	Type = EParkourType::Slide;

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	(*DataMap.Find(EParkourType::Slide))[0].PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Slide()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = NULL;
}

bool UCParkourComponent::Check_ObstacleMode(EParkourType InType, FParkourData& OutData)
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Ceil].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(InType);
	//datas->Num
	//(*datas).Num

	for (int32 i = 0; i < (*datas).Num(); i++)
	{
		bool b = true;
		b &= (*datas)[i].MinDistance < HitDistance;
		b &= (*datas)[i].MaxDistance > HitDistance;
		b &= FMath::IsNearlyEqual((*datas)[i].Extent, HitObstacleExtent.Y, 10);

		OutData = (*datas)[i];
		CheckTrueResult(b, true);
	}

	return false;
}

void UCParkourComponent::DoParkour_Obstacle(EParkourType InType, FParkourData& InData)
{
	Type = InType;

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	InData.PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Obstacle()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = NULL;
}

