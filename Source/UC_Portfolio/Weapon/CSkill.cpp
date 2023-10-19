#include "Weapon/CSkill.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CCamComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CInGameUIComponent.h"
#include "Components/CMontagesComponent.h"

UCSkill::UCSkill()
{

}

void UCSkill::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	Status = CHelpers::GetComponent<UCStatusComponent>(Owner);
	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
	Camera = CHelpers::GetComponent<UCCamComponent>(Owner);
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(Owner);
	GameUI = CHelpers::GetComponent<UCInGameUIComponent>(Owner);
}
