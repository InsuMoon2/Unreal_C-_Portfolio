#include "Weapon/AddOns/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/TimelineComponent.h"

ACGhostTrail::ACGhostTrail()
{
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");

	// Curve 가져오기
	CHelpers::GetAsset<UCurveFloat>(&GhostTrail_Curve, "CurveFloat'/Game/_Game/Weapon/AddOns/GhostTrail_Curve.GhostTrail_Curve'");
	// 타임라인 생성
	CHelpers::CreateActorComponent<UTimelineComponent>(this, &Timeline, "Timeline");
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	PlayGhostTrail();
	//Timeline->PlayFromStart();
}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	EndGhostTrail();
	//Timeline->FinishDestroy();
}

void ACGhostTrail::PlayGhostTrail()
{
	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/M_Trail_Inst.M_Trail_Inst'");

	Material = UMaterialInstanceDynamic::Create(material, this);
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exponent", Exponent);

	Owner = Cast<ACharacter>(GetOwner());

	Mesh->SetVisibility(false);
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
	Mesh->SetRelativeScale3D(Scale);

	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++)
	{
		Mesh->SetMaterial(i, Material);
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			if (Mesh->IsVisible() == false)
				Mesh->ToggleVisibility();

			float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			SetActorLocation(Owner->GetActorLocation() - FVector(ScaleAmount.X, ScaleAmount.Y, height - ScaleAmount.Z));
			SetActorRotation(Owner->GetActorRotation() + FRotator(0, -90, 0));

			Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
		});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);
}

void ACGhostTrail::EndGhostTrail()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ACGhostTrail::SubGhostTrail_Alpha()
{
	/*if (!!GhostTrail_Curve)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "PlayGhostTrail");

		Timeline->AddInterpFloat(GhostTrail_Curve, timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(1.0f);
	}*/
}


