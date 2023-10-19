#include "Weapon/SubActions/CSubAction_Guard.h"
#include "Global.h"
#include "Components/CCamComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCSubAction_Guard::Pressed()
{
	CheckFalse(State->IsIdleMode());

	Super::Pressed();

	State->SetGuardMode();
	State->OnParryingMode();

	ActionData.DoAction(Owner);
}

void UCSubAction_Guard::Released()
{
	// 공격 도중은 안풀리게 ? 
	CheckTrue(State->IsActionMode());

	Super::Released();

	if(State->IsGuardMode())
	{
		Owner->PlayAnimMontage(Guard_End);
		State->OffParryingMode();
	}
}

void UCSubAction_Guard::Begin_SubAction_Implementation()
{
	Super::Begin_SubAction_Implementation();

	State->SetGuardMode();
}

void UCSubAction_Guard::End_SubAction_Implementation()
{
	Super::End_SubAction_Implementation();

	//State->OffParryingMode();
}

void UCSubAction_Guard::OnAttachmentEndCollision()
{

}

void UCSubAction_Guard::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{

}
