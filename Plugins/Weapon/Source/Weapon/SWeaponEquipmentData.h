#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class WEAPON_API SWeaponEquipmentData
	: public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	static TSharedPtr<class SWeaponCheckBoxes> CreateCheckBoxes();

	void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
	static TSharedPtr<class SWeaponCheckBoxes> CheckBoxes;
};
