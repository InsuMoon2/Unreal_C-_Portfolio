#include "Components/CMontagesComponent.h"
#include "CCamComponent.h"
#include "CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

#define LOG_UCMontagesComponent 1

UCMontagesComponent::UCMontagesComponent()
{

}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");

		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	// Data Setting
	TArray<FMontagesData*> datas;
	DataTable->GetAllRows<FMontagesData>("", datas);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontagesData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		} // for(data)
	} // for(i)

//#if LOG_UCMontagesComponent
//	for(FMontagesData* data : datas)
//	{
//		if (data == nullptr)
//			continue;
//
//		FString str;
//		str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type));
//		str.Append(" / ");
//		str.Append(data->Montage->GetPathName());
//
//		CLog::Log(str);
//	}
//
//#endif
}

bool UCMontagesComponent::IsIncapacitation()
{
	return CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter)->IsIncapacitation();
}

void UCMontagesComponent::PlayDoubleJump()
{
	PlayAnimMontage(EStateType::Jump);
}

void UCMontagesComponent::PlayBackStepMode()
{
	PlayAnimMontage(EStateType::BackStep);
}

void UCMontagesComponent::PlayRollMode()
{
	PlayAnimMontage(EStateType::Roll, OwnerCharacter->InputComponent);
}

void UCMontagesComponent::PlayIncapacitation()
{
	PlayAnimMontage(EStateType::Incapacitation);
}

void UCMontagesComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayParryingHitted()
{
	OwnerCharacter->PlayAnimMontage(ParryingHitted);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);
	FMontagesData* data = Datas[(int32)InType];

	if (IsIncapacitation())
	{
		if (OwnerCharacter->GetCurrentMontage() != data->Montage)
			OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);

		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType, UInputComponent* Input)
{
	CheckNull(OwnerCharacter);
	FName section;

	UCCamComponent* cam;
	cam = CHelpers::GetComponent<UCCamComponent>(OwnerCharacter);

	FMontagesData* data = Datas[(int32)InType];

	if (Input->GetAxisValue("MoveForward") > 0.0f)
		section = "Front";

	else if (Input->GetAxisValue("MoveRight") > 0.0f)
	{
		section = "Right";

		cam->EnableControlRotation();
	}

	else if (Input->GetAxisValue("MoveRight") < 0.0f)
	{
		section = "Left";

		cam->EnableControlRotation();
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate, section);
}



