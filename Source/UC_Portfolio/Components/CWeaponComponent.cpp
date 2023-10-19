#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/CWeaponAsset.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CEquipment.h"
#include "Weapon/CDoAction.h"
#include "Weapon/CSkill.h"
#include "Weapon/CSubAction.h"
#include "Weapon/CWeaponData.h"

class UCStatusComponent;

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	for(int32 i = 0; i < (int32)EWeaponType::UnArmed; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);
	}

}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);

	if (!!GetAirAction())
		GetAirAction()->Tick(DeltaTime);

	if (!!GetSubAction())
		GetSubAction()->Tick(DeltaTime);

	// 스킬 쿨타임 때문에 무기 종류에 상관 없이 Tick을 사용
	{
		//for (int32 i = 0; i < Skill_MAX; i++)
	//{
	//	if (!!GetSkill(i))
	//		GetSkill(i)->Tick(DeltaTime);
	//}

		for (int32 i = 0; i < (int32)EWeaponType::UnArmed; i++)
		{
			if (!!Datas[i])
			{
				for (int32 j = 0; j < Skill_MAX; j++)
				{
					if (!!Datas[i]->GetSkill(j))
					{
						Datas[i]->GetSkill(j)->Tick(DeltaTime);
					}
				}
			}
		}
	}
	
}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

bool UCWeaponComponent::IsActionMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsActionMode();
}

bool UCWeaponComponent::IsAirActionMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsAirActionMode();
}

bool UCWeaponComponent::IsExecutionMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsExecution();
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)WeaponType], nullptr);

	return Datas[(int32)WeaponType]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)WeaponType], nullptr);

	return Datas[(int32)WeaponType]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)WeaponType], nullptr);

	return Datas[(int32)WeaponType]->GetDoAction();
}

UCDoAction* UCWeaponComponent::GetAirAction()
{
	CheckFalseResult(IsAirActionMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)WeaponType], nullptr);

	return Datas[(int32)WeaponType]->GetAirAction();
}

UCSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)WeaponType], nullptr);

	return Datas[(int32)WeaponType]->GetSubAction();
}

UCSkill* UCWeaponComponent::GetSkill(int32 InIndex)
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)WeaponType], nullptr);

	return Datas[(int32)WeaponType]->GetSkill(InIndex);
}

void UCWeaponComponent::SetUnarmedMode()
{
	CheckTrue(IsActionMode());
	GetEquipment()->Unequip();

	ChangeType(EWeaponType::UnArmed);
}


void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetDaggerMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Dagger);
}

void UCWeaponComponent::SetWizardMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Wizard);
}

void UCWeaponComponent::SetGruxWeaponMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::GruxWeapon);
}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::DoAction()
{
	if (IsAirActionMode())
	{
		if (!!GetAirAction())
		{
			GetAirAction()->DoAction();

			return;
		}
	}

	else
	{
		if (!!GetDoAction())
		{
			GetDoAction()->DoAction();

			return;
		}

	}
}

void UCWeaponComponent::SubAction_Pressed()
{
	if (!!GetSubAction())
		GetSubAction()->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
	if (!!GetSubAction())
		GetSubAction()->Released();
}

void UCWeaponComponent::Skill1_Pressed()
{
	if (!!GetSkill((int32)ESkillType::Skill1))
		GetSkill((int32)ESkillType::Skill1)->Skill_Pressed(ESkillType::Skill1);
}

void UCWeaponComponent::Skill1_Released()
{
	if (!!GetSkill((int32)ESkillType::Skill1))
		GetSkill((int32)ESkillType::Skill1)->Skill_Released(ESkillType::Skill1);
}

void UCWeaponComponent::Skill2_Pressed()
{
	if (!!GetSkill((int32)ESkillType::Skill2))
		GetSkill((int32)ESkillType::Skill2)->Skill_Pressed(ESkillType::Skill2);
}

void UCWeaponComponent::Skill2_Released()
{
	if (!!GetSkill((int32)ESkillType::Skill2))
		GetSkill((int32)ESkillType::Skill2)->Skill_Released(ESkillType::Skill2);
}

void UCWeaponComponent::Skill3_Pressed()
{
	if (!!GetSkill((int32)ESkillType::Skill3))
		GetSkill((int32)ESkillType::Skill3)->Skill_Pressed(ESkillType::Skill3);
}

void UCWeaponComponent::Skill3_Released()
{
	if (!!GetSkill((int32)ESkillType::Skill3))
		GetSkill((int32)ESkillType::Skill3)->Skill_Released(ESkillType::Skill3);
}

void UCWeaponComponent::Skill4_Pressed()
{
	if (!!GetSkill((int32)ESkillType::Skill4))
		GetSkill((int32)ESkillType::Skill4)->Skill_Pressed(ESkillType::Skill4);
}

void UCWeaponComponent::Skill4_Released()
{
	if (!!GetSkill((int32)ESkillType::Skill4))
		GetSkill((int32)ESkillType::Skill4)->Skill_Released(ESkillType::Skill4);
}

void UCWeaponComponent::Skill_AirAction()
{
	if (!!GetSkill((int32)ESkillType::Skill_Upper))
		GetSkill((int32)ESkillType::Skill_Upper)->Skill_Upper();
}

void UCWeaponComponent::Skill_Execution()
{
	if (!!GetSkill((int32)ESkillType::Skill_Execution))
		GetSkill((int32)ESkillType::Skill_Execution)->Skill_Execution();
}

void UCWeaponComponent::Skill_Parrying()
{
	if (!!GetSkill((int32)ESkillType::SKill_Parrying))
		GetSkill((int32)ESkillType::SKill_Parrying)->Skill_Parrying();
}

float UCWeaponComponent::SetCoolTime(float InCoolTime, float InMaxCoolTime, float InDeltaSecond)
{
	CoolTime = InCoolTime;
	MaxCoolTime = InMaxCoolTime;

	CoolTime += (InDeltaSecond * -1.0f);
	CoolTime = FMath::Clamp(CoolTime, 0.0f, MaxCoolTime);

	//CLog::Print(FString::Printf(TEXT("Print : %f"), CoolTime));

	return CoolTime;
}

FString UCWeaponComponent::GetWeaponTypeAsString()
{
	static const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	if (!EnumPtr)
	{
		return FString("Invalid");
	}

	FString StateString = EnumPtr->GetNameStringByValue(static_cast<int64>(GetWeapon()));

	return StateString;
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (WeaponType == InType)
	{
		SetUnarmedMode();

		return;
	}

	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if (!!Datas[(int32)InType])
	{
		Datas[(int32)InType]->GetEquipment()->Equip();

		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = WeaponType;
	WeaponType = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(prevType, InType);
}

