#include "Character/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/CWeaponStructures.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CZoomComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CTargetingComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_HUD.h"


ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	{
		CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
		CHelpers::CreateComponent<USpringArmComponent>(this, &SceneCaptureArm, "SceneCaptureArm", GetCapsuleComponent());

		CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
		CHelpers::CreateComponent<UChildActorComponent>(this, &MainCamChild, "MainCamChild", Camera);

		CHelpers::CreateComponent<UCameraComponent>(this, &SequenceCam, "SequenceCam", GetMesh());
		CHelpers::CreateComponent<UChildActorComponent>(this, &SequenceChild, "SequenceChild", SequenceCam);

		// Actor Components
		CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapons");
		CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
		CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
		CHelpers::CreateActorComponent<UCCamComponent>(this, &CameraControl, "CameraControl");
		CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
		CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
		CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");

		CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
		CHelpers::CreateActorComponent<UCTargetingComponent>(this, &Targeting, "Targeting");

		CHelpers::CreateActorComponent<UCInGameUIComponent>(this, &InGameUI, "HUD");

		// Actor Sequence
		//CHelpers::CreateActorComponent<UActorSequenceComponent>(this, &ActorSequence, "ActorSequence");

		TeamID = 1;
	}

	// Mesh, Animation Setting
	{
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

		USkeletalMesh* mesh;
		CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/DarkFantasyKnight/Meshes/Character/SK_DarkFantasyKnight.SK_DarkFantasyKnight'");
		GetMesh()->SetSkeletalMesh(mesh);

		TSubclassOf<UCAnimInstance> animInstance;
		CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/_Game/ABP_Character.ABP_Character_C'");
		GetMesh()->SetAnimClass(animInstance);
	}

	// SpringArm, Camera Setting
	{
		SpringArm->SetRelativeLocation(FVector(0, 0, 140));
		SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
		SpringArm->TargetArmLength = 200;
		SpringArm->bDoCollisionTest = false;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bEnableCameraLag = true;

		GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	}

	// Parkour Arrow Setting
	{
		CHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());
		Arrows.SetNum((int32)EParkourArrowType::Max);

		for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
		{
			FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
			CHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);

			switch ((EParkourArrowType)i)
			{
			case EParkourArrowType::Center:
				Arrows[i]->ArrowColor = FColor::Red;
				break;

			case EParkourArrowType::Ceil:
				Arrows[i]->ArrowColor = FColor::Green;
				Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
				break;

			case EParkourArrowType::Floor:
				Arrows[i]->ArrowColor = FColor::Blue;
				Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
				break;

			case EParkourArrowType::Left:
				Arrows[i]->ArrowColor = FColor::Magenta;
				Arrows[i]->SetRelativeLocation(FVector(0, -30, 0));
				break;

			case EParkourArrowType::Right:
				Arrows[i]->ArrowColor = FColor::Magenta;
				Arrows[i]->SetRelativeLocation(FVector(0, 30, 0));
				break;

			case EParkourArrowType::Land:
				Arrows[i]->ArrowColor = FColor::Yellow;
				Arrows[i]->SetRelativeLocation(FVector(200, 0, 100));
				Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
				break;
			}
		}
	}

	// Widget, HUD
	{
		
	}
	

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 기본 속도, 카메라 세팅
	Movement->OnRun();
	CameraControl->DisableControlRotation();
	InGameUI->InitPlayerHUD();

	// 시야 각도 제한
	Cast<APlayerController>(GetController())->PlayerCameraManager->ViewPitchMin = -50;
	Cast<APlayerController>(GetController())->PlayerCameraManager->ViewPitchMax = 40;

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InGameUI->UpdatePlayerHUD();
	Status->ChargeMana();

	// 구르기 보간
	FRotator CameraRotation = GetControlRotation();
	if (IsRolling == true)
	{
		FRotator rot = GetActorForwardVector().Rotation();
		FRotator NewRotation = FMath::RInterpTo(rot, CameraRotation,
			DeltaTime, 8.0f);

		SetActorRotation(FRotator(0, NewRotation.Yaw, 0));
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Camera
	PlayerInputComponent->BindAxis("VerticalLook", CameraControl, &UCCamComponent::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", CameraControl, &UCCamComponent::OnHorizontalLook);

	//PlayerInputComponent->BindAxis("Zoom", CameraControl, &UCCamComponent::ApplyZoom);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::SetZooming);

	// Moving
	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);
	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Pressed, this, &ACPlayer::Jump);
	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);

	// Weapon Binding
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Dagger", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetDaggerMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);
	PlayerInputComponent->BindAction("Wizard", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWizardMode);

	// Action
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	// SubAction
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnRightButton);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffRightButton);

	PlayerInputComponent->BindAction("AirAction", EInputEvent::IE_Pressed, this, &ACPlayer::AirAction);

	// Skill
	PlayerInputComponent->BindAction("Skill1", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Skill1_Pressed);
	PlayerInputComponent->BindAction("Skill1", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::Skill1_Released);

	PlayerInputComponent->BindAction("Skill2", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Skill2_Pressed);
	PlayerInputComponent->BindAction("Skill2", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::Skill2_Released);

	PlayerInputComponent->BindAction("Skill3", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Skill3_Pressed);
	PlayerInputComponent->BindAction("Skill3", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::Skill3_Released);

	PlayerInputComponent->BindAction("Skill4", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Skill4_Pressed);
	PlayerInputComponent->BindAction("Skill4", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::Skill4_Released);

	// Sub Skill
	PlayerInputComponent->BindAction("Execution", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Skill_Execution);
	//PlayerInputComponent->BindAction("Parrying", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Skill_Parrying);

	// Targeting
	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, Targeting, &UCTargetingComponent::Toggle_Target);
	PlayerInputComponent->BindAction("Right_Target", EInputEvent::IE_Pressed, Targeting, &UCTargetingComponent::Right_Focus);
	PlayerInputComponent->BindAction("Left_Target", EInputEvent::IE_Pressed, Targeting, &UCTargetingComponent::Left_Focus);

}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Idle: 
		Movement->Move();
		State->OffParryingMode();
		
		break;
	case EStateType::BackStep: BackStep(); break;
	case EStateType::Roll: Roll(); break;
	case EStateType::Hitted: Hitted();
		State->OffAirActionMode();
		break;

	}
}


void ACPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bJump = false;
	double_Jump = 0;

	bool bCheck = true;
	bCheck &= State->IsJumpMode();

	if (bCheck)
		State->SetIdleMode();

	Parkour->DoParkour(true);
}

void ACPlayer::OnAvoid()
{
	CheckTrue(State->IsBackstepMode());
	CheckTrue(State->IsRollMode());
	CheckTrue(State->IsJumpMode());
	CheckTrue(State->IsAirActionMode());
	//CheckTrue(State->IsHittedMode());
	CheckFalse(Movement->CanMove());

	CheckFalseResult(
		Status->CompareInMana(OnAvoidMana),
		InGameUI->ShowLackText(true));

	Status->UseMana(OnAvoidMana);

	CameraControl->DisableControlRotation();

	// 입력 방향이 없다면 앞구르기
	if (InputComponent->GetAxisValue("MoveForward") == 0.0f &&
		InputComponent->GetAxisValue("MoveRight") == 0.0f)
	{
		InputDirection = GetActorForwardVector();
	}

	else
		InputDirection = GetCharacterMovement()->GetLastInputVector();

	RollDirection = InputDirection.Rotation();

	if (Movement->CanMove())
		RollRotation = FRotator(RollDirection.Pitch, RollDirection.Yaw + 180.0f, RollDirection.Roll);

	else
		RollRotation = FRotator(RollDirection.Pitch, RollDirection.Yaw, RollDirection.Roll);
	
	// Back
	if (InputComponent->GetAxisValue("MoveForward") < 0.0f)
	{
		State->SetBackStepMode();

		SetActorRotation(RollRotation);
	}
	// Forward, Right, Left
	else
	{
		State->SetRollMode();
	}
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Casuer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHittedMode();

	return damage;
}

void ACPlayer::Hitted()
{
	// Apply Damage
	{
		Status->Damage(Damage.Power);
		// Power 비워주기
		Damage.Power = 0;
		Movement->Stop();
	}

	FTimerDelegate MovingDelegate;
	MovingDelegate.BindUFunction(this, "CanMoving");

	GetWorld()->GetTimerManager().SetTimer(
		Moving_TimerHandle, MovingDelegate, 0.5f, false);

	// Montage, HitStop, SoundWave
	{
		if (!!Damage.Event && !!Damage.Event->HitData)
		{
			FHitData* data = Damage.Event->HitData;

			// 스킬 시전 도중 맞으면 몽타주 플레이 X
			if (!State->IsSkillMode())
				data->PlayMontage(this);

			data->PlayHitStop(GetWorld());
			data->PlaySoundWave(this);
			data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();

			// 방향
			FVector direction = target - start;
			direction.Normalize();

			FVector launch_XY = (-direction * data->Launch.X) + (-direction * data->Launch.Y);
			FVector launch_Z = GetActorUpVector() * data->Launch.Z;

			if (!State->IsSkillMode())
			{
				// 죽지 않았을 때만 Launch가 되게
				if (Status->IsDead() == false)
				{
					LaunchCharacter(launch_XY + launch_Z, true, true);

					// TODO :: Launch 값 출력
					//CLog::Print(FVector(launch_XY + launch_Z));

					FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

					// Yaw 값으로만 회전하게
					SetActorRotation(FRotator(0.0f, targetRotation.Yaw, 0.0f));

					return;
				}
			}
			
		}
	}

	// Dead
	if (Status->IsDead())
	{
		State->SetDeadMode();

		return;
	}
	

	Damage.Character = nullptr;
	Damage.Casuer = nullptr;
	Damage.Event = nullptr;
}

void ACPlayer::CanMoving()
{
	Movement->Move();
	GetWorld()->GetTimerManager().ClearTimer(Moving_TimerHandle);
}

void ACPlayer::End_Hitted()
{
	IICharacter::End_Hitted();

	//Movement->Move();
	State->SetIdleMode();
}

void ACPlayer::End_Guard()
{
	IICharacter::End_Guard();

	State->SetGuardMode();
}

void ACPlayer::Jump()
{
	CheckTrue(State->IsBackstepMode());
	CheckTrue(State->IsRollMode());

	Super::Jump();

	if (double_Jump < 2)
	{
		double_Jump++;	

		if (State->IsJumpMode())
		{
			Montages->PlayDoubleJump();
			LaunchCharacter(FVector(0.0f, 0.0f, 600.0f), false, true);

			return;
		}
			
		bJump = true;
		State->SetJumpMode();
	}
}

void ACPlayer::Roll()
{
	SetActorRotation(RollDirection);

	Montages->PlayRollMode();
}

void ACPlayer::BackStep()
{
	//Movement->Stop();
	Montages->PlayBackStepMode();
}

void ACPlayer::End_BackStep()
{
	if (!Weapon->IsUnarmedMode())
		CameraControl->EnableControlRotation();

	else
		CameraControl->DisableControlRotation();

	// 보간 제어
	IsRolling = false;

	//Movement->Move();
	State->SetIdleMode();
}

void ACPlayer::AirAction()
{
	CheckFalse(State->IsIdleMode());
	Weapon->Skill_AirAction();
}

void ACPlayer::Do_Parkour()
{
	CheckTrue(Parkour->IsExecuting());

	//CLog::Print(Parkour->IsExecuting());

	Parkour->DoParkour();
	State->SetParkourMode();
}

void ACPlayer::SetZooming(float InValue)
{
	CheckTrue(Weapon->IsBowMode());

	Zoom->SetZoomValue(InValue);
}

void ACPlayer::OnRightButton()
{
	switch (Weapon->GetWeapon())
	{
	case EWeaponType::UnArmed: 
		Do_Parkour();
		break;

	case EWeaponType::Bow:
		Zoom->SetComponentTickEnabled(false);
		Weapon->SubAction_Pressed();
		break;

	case EWeaponType::Hammer:
		break;

	case EWeaponType::Sword:
		Weapon->SubAction_Pressed(); break;

	case EWeaponType::Dagger:
		break;

	case EWeaponType::Wizard:
		Weapon->SubAction_Pressed(); break;
	}

}

void ACPlayer::OffRightButton()
{
	switch (Weapon->GetWeapon())
	{
	case EWeaponType::Bow:
		Zoom->SetComponentTickEnabled(true);
		Weapon->SubAction_Released();
		break;

	case EWeaponType::UnArmed:
	case EWeaponType::Sword:
		Weapon->SubAction_Released();break;

	case EWeaponType::Hammer:
		break;

	case EWeaponType::Dagger:
		break;

	case EWeaponType::Wizard:
		Weapon->SubAction_Released(); break;
	}
	
}

AActor* ACPlayer::IsMain_Camera_Child()
{
	return MainCamChild->GetChildActor();
}

AActor* ACPlayer::IsSequence_Child()
{
	return SequenceChild->GetChildActor();
}
