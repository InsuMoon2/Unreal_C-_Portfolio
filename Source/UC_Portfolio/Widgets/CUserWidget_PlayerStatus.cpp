#include "Widgets/CUserWidget_PlayerStatus.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"

void UCUserWidget_PlayerStatus::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	WeaponComponent = CHelpers::GetComponent<UCWeaponComponent>(character);

	WeaponComponent->OnWeaponTypeChange.AddDynamic(this, &UCUserWidget_PlayerStatus::OnWeaponTypeChanged);
}
