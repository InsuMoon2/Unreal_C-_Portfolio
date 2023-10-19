#include "Weapon/SubActions/CSubAction_Bow.h"

#include "AIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Weapon/Attachments/CAttachment_Bow.h"
#include "Widgets/CUserWidget_CrossHair.h"
#include "DrawDebugHelpers.h"
#include "Character/CPlayer.h"
#include "Character/Enemies/CEnemy.h"

UCSubAction_Bow::UCSubAction_Bow()
{
	

	CHelpers::GetAsset<UCurveVector>(&Curve, "CurveVector'/Game/_Game/Weapon/Bow/Curve_Aiming.Curve_Aiming'");
	CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/_Game/Widget/WB_CrossHair.WB_CrossHair_C'");
}

void UCSubAction_Bow::Pressed()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsSubActionMode());

	if (!!Owner->GetController<AAIController>())
	{
		Super::Pressed();
		State->OnSubActionMode();

		return;

	}

	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Pressed();
	State->OnSubActionMode();

	// TODO :: 사운드 문제 있음. 수정해야함
	UGameplayStatics::PlaySound2D(Owner->GetWorld(), BendingSound);

	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	if (!!CrossHairClass)
	{
		CrossHair->SetVisibility(ESlateVisibility::Visible);
	}

	Timeline.PlayFromStart();
}

void UCSubAction_Bow::Released()
{
	CheckFalse(State->IsSubActionMode());

	if (!!Owner->GetController<AAIController>())
	{
		Super::Released();
		State->OffSubActionMode();

		return;

	}

	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Released();
	State->OffSubActionMode();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	if (!!CrossHairClass)
	{
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}

	Timeline.ReverseFromEnd();
}

void UCSubAction_Bow::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);

	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);

	// Bend 길이 늘리기
	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	if (!!bow)
	{
		Bend = bow->GetBend();
	}

	// CrossHair 색 변경
	if (!!CrossHairClass)
	{
		// 플레이어인지
		ACPlayer* player = Cast<ACPlayer>(Owner);

		if (!!player)
		{
			CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(Owner->GetController<APlayerController>(), CrossHairClass);

			// Player인지 구별하기 위해
			if (!!CrossHair)
			{
				CrossHair->AddToViewport();
				CrossHair->SetVisibility(ESlateVisibility::Hidden);
				CrossHair->CrossHair_White();
			}
		}
	}

}

void UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);
	//CLog::Print(bInAction);

	if (bInAction)
	{
		FVector bone_Location = Owner->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow");

		// Get Player controller
		AController* Controller = nullptr;
		Controller = UGameplayStatics::GetPlayerController(Owner->GetWorld(), 0);

		if (!!Controller)
		{
			FRotator ControllerRotation = Controller->GetControlRotation();
			FVector controller_Location = ControllerRotation.Vector();

			FHitResult hitResults;
			// Bone Location 위치
			FVector Start = bone_Location;
			FVector End = bone_Location + controller_Location * Distance;
			// Player 위치
			float half_Height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 20.0f;
			FVector Player_Start = Owner->GetActorLocation() + (Owner->GetActorRightVector() * 35.0f) + (Owner->GetActorUpVector() * half_Height);
			FVector Player_End = Player_Start + controller_Location * Distance;

			TArray<AActor*> ignores;
			ignores.Add(Owner);

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

			// Player가 아니면 밑에 부분은 그냥 종료
			CheckNull(Cast<ACPlayer>(Owner));

			if (DrawDebug == EDrawDebugTrace::ForOneFrame)
			{
				DrawDebugDirectionalArrow(Owner->GetWorld(), Player_Start, Player_End, 25, FColor::Green, true,
					1, 0, 1);
			}

			UKismetSystemLibrary::LineTraceSingleForObjects(Owner->GetWorld(), Player_Start, Player_End, ObjectTypes,
				false, ignores, DrawDebug, hitResults, true);

			if (hitResults.bBlockingHit)
				CrossHair->CrossHair_Red();

			else
				CrossHair->CrossHair_White();

			//CLog::Print(hitResults.bBlockingHit);
		}
	}
}

void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;

	if (!!Bend)
		*Bend = Output.Y;
}
