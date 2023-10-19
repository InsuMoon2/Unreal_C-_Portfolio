#include "Cinematics/CCinematicTrigger.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Components/BoxComponent.h"

ACCinematicTrigger::ACCinematicTrigger()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Cinematic_Trigger, "Cinematic_Trigger", Root);


}

void ACCinematicTrigger::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ACCinematicTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCinematicTrigger::OnComponentBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ACCinematicTrigger::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACCinematicTrigger::OnFinish_BossSequence()
{

}