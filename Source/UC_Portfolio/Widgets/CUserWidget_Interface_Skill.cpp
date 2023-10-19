#include "Widgets/CUserWidget_Interface_Skill.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"

void UCUserWidget_Interface_Skill::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	WeaponComponent = CHelpers::GetComponent<UCWeaponComponent>(character);

	WeaponComponent->OnWeaponTypeChange.AddDynamic(this, &UCUserWidget_Interface_Skill::OnWeaponTypeChanged);
}
