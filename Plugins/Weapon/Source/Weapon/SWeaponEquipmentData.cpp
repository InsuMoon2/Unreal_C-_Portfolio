#include "SWeaponEquipmentData.h"
#include "IPropertyUtilities.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "SWeaponCheckBoxes.h"
#include "WeaponStyle.h"
#include "DetailWidgetRow.h"

TSharedPtr<class SWeaponCheckBoxes> SWeaponEquipmentData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SWeaponEquipmentData::MakeInstance()
{
	return MakeShareable(new SWeaponEquipmentData());
}

TSharedPtr<SWeaponCheckBoxes> SWeaponEquipmentData::CreateCheckBoxes()
{
	// 생성하고 대입해서 그대로 리턴하는 것이다.
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset();
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SWeaponCheckBoxes());
}

void SWeaponEquipmentData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow,
                                           IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

	InHeaderRow
	.NameContent()
	[
		InPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(FWeaponStyle::Get()->DesiredWidth.X)
	.MaxDesiredWidth(FWeaponStyle::Get()->DesiredWidth.Y)
	[
		CheckBoxes->Draw()
	];
}

void SWeaponEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}
