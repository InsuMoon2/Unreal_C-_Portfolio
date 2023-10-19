#include "Weapon/Skills/Hammer/CSkill_Hammer_C_ShockWave.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Character/Enemies/CEnemy_Boss.h"
#include "Particles/ParticleSystem.h"

ASkill_CHammer_C_ShockWave::ASkill_CHammer_C_ShockWave()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Root);

	UNiagaraSystem* niagara;
	CHelpers::GetAsset<UNiagaraSystem>(&niagara, "NiagaraSystem'/Game/SuperheroFlight/VFX/Niagara/System/SuperheroLanding/Hammer_Exe.Hammer_Exe'");
	Niagara->SetAsset(niagara);
	Niagara->SetRelativeLocation(FVector(0.0f, 0.0f, -108.1770f));
}

void ASkill_CHammer_C_ShockWave::BeginPlay()
{
	Super::BeginPlay();

	// Niagara 끝날 때 호출
	Niagara->OnSystemFinished.AddDynamic(this, &ASkill_CHammer_C_ShockWave::OnSystemFinished);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ASkill_CHammer_C_ShockWave::OnComponentBeginOverlap);
}

void ASkill_CHammer_C_ShockWave::OnSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

void ASkill_CHammer_C_ShockWave::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if(!!character)
	{
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
		CheckTrue(state->IsDeadMode());
		CheckTrue(CHelpers::IsSameTeam(Cast<ACharacter>(GetOwner()), character));
	}

	if (!!character)
	{
		ACharacter* Boss = Cast<ACEnemy_Boss>(character);
		if (!!Boss)
		{
			Hitted = Boss;
			BossHitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted);

			return;
		}

		Hitted = character;
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted);
	}
}

void ASkill_CHammer_C_ShockWave::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Hitted = nullptr;
}
