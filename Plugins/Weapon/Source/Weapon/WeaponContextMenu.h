#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"

class WEAPON_API FWeaponContextMenu
	: public FAssetTypeActions_Base
{
public:
	FWeaponContextMenu(EAssetTypeCategories::Type InCategory);

public:
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	uint32 GetCategories() override;

	void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;

private:
	// 고급 에셋 생성에 나올 카테고리 목록에 보여질 것이다.
	EAssetTypeCategories::Type Category;

};
