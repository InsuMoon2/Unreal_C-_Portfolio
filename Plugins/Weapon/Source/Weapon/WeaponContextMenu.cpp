#include "WeaponContextMenu.h"
#include "WeaponAssetEditor.h"
#include "Weapon/CWeaponAsset.h"

FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InCategory)
{
	Category = InCategory;
}

FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("DataAsset");
}

UClass* FWeaponContextMenu::GetSupportedClass() const
{
	return UCWeaponAsset::StaticClass();
}

FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Blue;
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category;
}

void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	/*for(UObject* obj : InObjects)
	{
		if (!!obj)
			GLog->Log(obj->GetName());
	}*/

	// 하나도 선택이 되어있지 않다면, 웨폰 에디터 만들어 놓은 것을 사용할 것이다.
	if (InObjects.Num() < 1)
		return;

	// 더블클릭을 해서 열 것이니까 하나만 선택이 되어있다고 가정할 것이다. 
	FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());
}
