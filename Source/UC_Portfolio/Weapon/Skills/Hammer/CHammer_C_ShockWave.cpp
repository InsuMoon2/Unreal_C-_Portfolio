#include "Weapon/Skills/Hammer/CHammer_C_ShockWave.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ACHammer_C_ShockWave::ACHammer_C_ShockWave()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Root);
}

void ACHammer_C_ShockWave::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACHammer_C_ShockWave::OnComponentBeginOverlap);
}

void ACHammer_C_ShockWave::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
	{
		Hitted = character;
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted);
		//CLog::Print(Hitted);
	}
}

void ACHammer_C_ShockWave::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Hitted = nullptr;
}
