#include "SWeaponAirActionData.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "SWeaponCheckBoxes.h"
#include "WeaponStyle.h"
#include "DetailWidgetRow.h"

TArray<TSharedPtr<class SWeaponCheckBoxes>> SWeaponAirActionData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SWeaponAirActionData::MakeInstance()
{
	return MakeShareable(new SWeaponAirActionData());
}

TSharedPtr<SWeaponCheckBoxes> SWeaponAirActionData::AddCheckBoxes()
{
	TSharedPtr<SWeaponCheckBoxes> checkBoxes = MakeShareable(new SWeaponCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SWeaponAirActionData::EmptyCheckBoxes()
{
	for (TSharedPtr<SWeaponCheckBoxes> ptr : CheckBoxes)
	{
		if (ptr.IsValid())
			ptr.Reset();
	}

	CheckBoxes.Empty();
}

void SWeaponAirActionData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow,
	IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SWeaponCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()))
	{
		InHeaderRow
			.NameContent()
			[
				InPropertyHandle->CreatePropertyNameWidget()
			]
		.ValueContent()
			.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
			.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
			[
				InPropertyHandle->CreatePropertyValueWidget()
			];

		return;
	}

	int32 index = InPropertyHandle->GetIndexInArray();
	CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

	// Display Name
	FString name = InPropertyHandle->GetPropertyDisplayName().ToString();
	name = "AirAction Data - " + name;

	InHeaderRow
		.NameContent()
		[
			SNew(SBorder)
			.BorderImage(FWeaponStyle::Get()->Array_Image.Get())
		[
			InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
		]

		]
	.ValueContent()
		.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
		.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
		[
			CheckBoxes[index]->Draw(true)
		];
}

void SWeaponAirActionData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	uint32 number = 0;
	InPropertyHandle->GetNumChildren(number);

	for (uint32 i = 0; i < number; i++)
	{
		TSharedPtr<IPropertyHandle> handle = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildBuilder.AddProperty(handle.ToSharedRef());

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
				value.ToSharedRef()
			];
	}//for(i)

	int32 index = InPropertyHandle->GetIndexInArray();
	CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);
}
