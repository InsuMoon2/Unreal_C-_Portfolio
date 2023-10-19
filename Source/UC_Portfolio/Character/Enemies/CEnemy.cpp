#include "Character/Enemies/CEnemy.h"
#include "Global.h"
#include "Character/CAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStatusComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Weapon/CWeaponStructures.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Widgets/CUserWidget_CrossHair.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Actor Component
	//CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	//CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");

	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", RootComponent);
	Box->SetWorldLocation(GetActorLocation());
	Box->SetBoxExtent(FVector(100.0f));
	Box->SetHiddenInGame(false);
	Box->SetVisibility(false);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/_Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	//Widget
	CHelpers::CreateComponent<UWidgetComponent>(this, &CrossHair, "CrossHair", RootComponent);

	TSubclassOf<UCUserWidget_CrossHair> crosshair;
	CHelpers::GetClass<UCUserWidget_CrossHair>(&crosshair, "WidgetBlueprint'/Game/_Game/Widget/WB_Targeting.WB_Targeting_C'");

	CrossHair->SetWidgetClass(crosshair);
	CrossHair->SetWidgetSpace(EWidgetSpace::Screen);
	CrossHair->SetRelativeLocation(FVector(20.0f, 0.0f, 40.0f));
	CrossHair->SetVisibility(false);

	// 처형 카메라 세팅
	{
		//SpringArm->SetRelativeLocation(FVector(0, 0, 140));
		//SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
		//SpringArm->TargetArmLength = 200;
		//SpringArm->bDoCollisionTest = false;
		//SpringArm->bUsePawnControlRotation = true;
		//SpringArm->bEnableCameraLag = true;

		GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	}
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Speed Setting
	Movement->OnWalk();

	// Color Setting
	Create_DynamicMaterial(this);
	Change_Color(this, OriginColor);

	// State Change
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACEnemy::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACEnemy::ACEnemy::OnComponentEndOverlap);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Status->ChargeMana();
	//CLog::Print(State->IsExecution());
}

void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
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

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch(InNewType)
	{
	case EStateType::Idle: 
		Movement->Move();
		Execution_Clear();

		break;

	case EStateType::Hitted: Hitted();
		break;

	case EStateType::Dead: Dead();
		break;

	case EStateType::Incapacitation: 
		Incapacitation();
		break;
	}
}

void ACEnemy::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// Landed되면 Profile을 원래대로
	{
		FName name = GetCapsuleComponent()->GetCollisionProfileName();
		if (name == TEXT("AirHitted"))
		{
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
			GetCharacterMovement()->GravityScale = 1.0f;
		}
	}
}

void ACEnemy::Hitted()
{
	// 구를 때 데미지 안받게
	CheckTrue(State->IsRollMode());
	CheckTrue(State->IsDeadMode());

	// Apply Damage
	{
		Status->Damage(Damage.Power);
		// Power 비워주기
		Damage.Power = 0;
	}

	// Change Color
	{
		Change_Color(this, FLinearColor::Red);

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "RestoreColor");

		GetWorld()->GetTimerManager().SetTimer(RestoreColor_TimerHandle, timerDelegate, 0.2f, false);
	}

	// Montage, HitStop, SoundWave
	{
		if (!!Damage.Event && !!Damage.Event->HitData)
		{
			FHitData* data = Damage.Event->HitData;

			data->PlayHitStop(GetWorld());
			data->PlayCameraShake(GetWorld());
			data->PlaySoundWave(this);
			data->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

			// Dead
			if (Status->IsDead())
			{
				State->SetDeadMode();

				return;
			}

			// Incapacitation
			if (Status->IsIncapacitation())
			{
				if (State->IsExecution())
					return;

				State->SetIncapacitationMode();

				return;
			}

			// 스킬 시전 도중 맞으면 몽타주 플레이 X
			if (!State->IsSkillMode())
				data->PlayMontage(this);

			FVector start = GetActorLocation();
			FVector target = Damage.Character->GetActorLocation();

			//방향벡터
			FVector direction = target - start;
			direction.Normalize();

			FVector launch_XY = (-direction * data->Launch.X) + (-direction * data->Launch.Y);
			FVector launch_Z = GetActorUpVector() * data->Launch.Z;

			// 죽지 않았을 때만 Launch가 되게
			if (Status->IsDead() == false && !Status->IsIncapacitation())
			{
				LaunchCharacter(launch_XY + launch_Z, false, true);

				// TODO :: Launch 값 출력
				//CLog::Print(FVector(launch_XY + launch_Z));
				FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

				// Yaw 값으로만 회전하게
				SetActorRotation(FRotator(0.0f, targetRotation.Yaw, 0.0f));
			}
		}
	}

	

	Damage.Character = nullptr;
	Damage.Casuer = nullptr;
	Damage.Event = nullptr;
}


void ACEnemy::End_Hitted()
{
	IICharacter::End_Hitted();

	State->SetIdleMode();
}

void ACEnemy::End_Incapacitation()
{
	IICharacter::End_Incapacitation();

	// 무력화 게이지 초기화, 상태 초기화 
	Status->SetMaxIncapacitation();
	State->SetIdleMode();

}

void ACEnemy::RestoreColor()
{
	Change_Color(this, OriginColor);
	GetWorld()->GetTimerManager().ClearTimer(RestoreColor_TimerHandle);
}

void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->PlayDeadMode();
}

void ACEnemy::Incapacitation()
{
	if (Status->GetHealth() >= 5.0f)
	{
		Box->SetVisibility(false);
		Montages->PlayIncapacitation();
	}

}

void ACEnemy::Execution_Clear()
{
	Box->SetVisibility(false);
	State->OffExecutionMode();

	if (Attacker != nullptr)
	{
		UCStateComponent* Attacker_State = CHelpers::GetComponent<UCStateComponent>(Attacker);
		Attacker_State->OffExecutionMode();

		Attacker = nullptr;
	}
}

void ACEnemy::End_Dead()
{
	IICharacter::End_Dead();

	Destroy();
}

void ACEnemy::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		Attacker = Cast<ACharacter>(OtherActor);
		CheckNull(Attacker);
		CheckFalse(State->IsIncapacitationMode());

		//State->SetIdleMode();

		UCInGameUIComponent* Attacker_UI = CHelpers::GetComponent<UCInGameUIComponent>(Attacker);
		Attacker_UI->ShowExecutionText(true);

		UCStateComponent* Attacker_State = CHelpers::GetComponent<UCStateComponent>(Attacker);
		Attacker_State->OnExecutionMode();
	}
}

void ACEnemy::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Attacker = Cast<ACharacter>(OtherActor);
	CheckNull(Attacker);

	UCInGameUIComponent* Attacker_UI = CHelpers::GetComponent<UCInGameUIComponent>(Attacker);
	Attacker_UI->ShowExecutionText(false);

	UCStateComponent* Attacker_State = CHelpers::GetComponent<UCStateComponent>(Attacker);
	Attacker_State->OffExecutionMode();

	Attacker = nullptr;
}

