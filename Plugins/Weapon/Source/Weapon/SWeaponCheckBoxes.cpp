#include "SWeaponCheckBoxes.h"
#include "WeaponStyle.h"
#include "SWeaponDetailsView.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "Camera/CameraShake.h"

void SWeaponCheckBoxes::AddProperties(TSharedPtr<IPropertyHandle> InHandle)
{
	uint32 number = 0;
	InHandle->GetNumChildren(number);

	for (uint32 i = 0; i < number; i++)
		InternalDatas.Add(FInternalData(InHandle->GetChildHandle(i)));
}

TSharedRef<SWidget> SWeaponCheckBoxes::Draw(bool bBackground)
{
	TSharedPtr<SUniformGridPanel> panel;
	SAssignNew(panel, SUniformGridPanel);
	panel->SetMinDesiredSlotWidth(150);

	for(int32 i = 0; i < InternalDatas.Num(); i++)
	{
		panel->AddSlot(i, 0)
		[
			SNew(SCheckBox)
			.IsChecked(InternalDatas[i].bChecked)
			.OnCheckStateChanged(this, &SWeaponCheckBoxes::OnCheckStateChanged, i)
			[
				SNew(STextBlock)
				.Text(FText::FromString(InternalDatas[i].Name))
			]
		];
	}

	if(bBackground == false)
		return panel.ToSharedRef();


	TSharedPtr<SBorder> border = SNew(SBorder)
		.BorderImage(FWeaponStyle::Get()->Array_Image.Get())
		[
			panel.ToSharedRef()
		];

	return border.ToSharedRef();
}

void SWeaponCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder* InChildrenBuilder)
{
	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		if (InternalDatas[i].bChecked == false)
			continue;

		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildrenBuilder->AddProperty(handle.ToSharedRef());

		TSharedPtr<SWidget> name;
		TSharedPtr<SWidget> value;

		row.GetDefaultWidgets(name, value);

		row.CustomWidget()
			.NameContent()
			[
				name.ToSharedRef()
		]
		.ValueContent()
			.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
			.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
			[
				// 기본자료형만 지원할 뿐 다른 자료형을 지원하지는 않는다.
				// 간단히 띄워주기만 하기 위한 모형임.
				//handle->CreatePropertyValueWidget()

				value.ToSharedRef()
		];
	}
}

void SWeaponCheckBoxes::SetUtilities(TSharedPtr<IPropertyUtilities> InUtilities)
{
	Utilities = InUtilities;
}

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
	InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;

	SWeaponDetailsView::OnRefreshByCheckBoxes();
	{
		Utilities->ForceRefresh();
	}
	SWeaponDetailsView::OffRefreshByCheckBoxes();
}

bool SWeaponCheckBoxes::CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount)
{
	bool bCheck = true;
	bCheck &= InCount > 0;

	int32 index = InHandle->GetIndexInArray();
	bCheck &= index >= 0;
	bCheck &= index < InCount;

	return bCheck;
}

void SWeaponCheckBoxes::CheckDefaultObject(int32 InIndex, UObject* InValue)
{
	UObject* val = nullptr;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (!!val && InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultObject(int32 InIndex, USoundCue* InValue)
{
	USoundCue* val = nullptr;
	InternalDatas[InIndex].Handle->GetValue(reinterpret_cast<UObject*&>(val));

	if (val != nullptr && InValue != val)
	{
		InternalDatas[InIndex].bChecked = true;
	}
}

void SWeaponCheckBoxes::CheckDefaultObejct(int32 InIndex, TSubclassOf<UMatineeCameraShake> CameraShakeClass)
{
	UObject* Obj = nullptr;
	InternalDatas[InIndex].Handle->GetValue(reinterpret_cast<UObject*&>(Obj));

	TSubclassOf<UMatineeCameraShake> val = Cast<UClass>(Obj);

	if (val && CameraShakeClass != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, float InValue)
{
	float val = 0.0f;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;

}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, bool InValue)
{
	bool val = false;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector& InValue)
{
	FVector val = FVector::ZeroVector;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, FVector& InValue)
{
	FVector val = FVector::ZeroVector;
	InternalDatas[InIndex].Handle->GetValue(val);

	if (InValue != val)
		InternalDatas[InIndex].bChecked = true;
}
