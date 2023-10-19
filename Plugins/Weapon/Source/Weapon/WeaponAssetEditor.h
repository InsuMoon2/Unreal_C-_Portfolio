#pragma once

#include "CoreMinimal.h"
#include "SWeaponLeftArea.h"
#include "Toolkits/AssetEditorToolkit.h"

class WEAPON_API FWeaponAssetEditor
	: public FAssetEditorToolkit
{
public:
	static void OpenWindow(FString InAssetName = "");
	static void Shutdown();

private:
	static TSharedPtr<FWeaponAssetEditor> Instance;

private:
	void Open(FString InAssetName);

protected:
	bool OnRequestClose() override;

public:
	void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

private:
	TSharedRef<SDockTab> Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> Spawn_DetailsViewTab(const FSpawnTabArgs& InArgs);

private:
	void OnListViewSelectedItem(FWeaponRowDataPtr InDataPtr);

public:
	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;
	FString GetWorldCentricTabPrefix() const override;
	FLinearColor GetWorldCentricTabColorScale() const override;

private:
	TSharedPtr<class SWeaponLeftArea> LeftArea;
	// DetailView의 최상의 자료형 IDetailsView
	TSharedPtr<class IDetailsView> DetailsView;

private:
	static const FName EditorName;
	static const FName LeftAreaTabId;
	static const FName DetailTabId;

private:
	FReply OnClicked();
};
