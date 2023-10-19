#include "WeaponAssetEditor.h"
#include "SWeaponDetailsView.h"
#include "SWeaponLeftArea.h"
#include "SWeaponEquipmentData.h"
#include "SWeaponHitData.h"
#include "SWeaponDoActionData.h"
#include "Weapon/CWeaponAsset.h"
#include "SWeaponAirActionData.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";
const FName FWeaponAssetEditor::LeftAreaTabId = "LeftArea";
const FName FWeaponAssetEditor::DetailTabId = "Details";

TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;

void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	//창이 한번이라도 열리면 인스턴스가 할당이 될 것이다.
	if(Instance.IsValid())
	{
		//창이 열려있는 상태라면
		if(Instance->LeftArea.IsValid())
		{
			FWeaponRowDataPtr ptr = nullptr;

			// 컨텐츠 브라우저를 더블클릭을 했다면 AssetName으로 ptr을 찾아온다.
			if (InAssetName.Len() > 0)
				ptr = Instance->LeftArea->GetRowDataPtrByName(InAssetName);

			// Ptr이 없다면 FirstData 첫번째 데이터를 선택되게 열어주면 될 것이다.
			if (ptr.IsValid() == false)
				ptr = Instance->LeftArea->GetFirstDataPtr();

			// 찾았다면 LeftArea의 SelectDataPtr에서 ptr->Asset을 주면 될 것이다.
			Instance->LeftArea->SelectDataPtr(ptr->Asset);

			// 그렇게하고 더 이상 수행할 필요가 없으니까 return을 해준 것이다.
			return;
		}

		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FWeaponAssetEditor::Shutdown()
{
	if (Instance.IsValid())
	{
		Instance->CloseWindow();

		Instance.Reset();
		Instance = nullptr;
	}
}

void FWeaponAssetEditor::Open(FString InAssetName)
{
	LeftArea = SNew(SWeaponLeftArea)
	.OnSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//DetailsView
	{
		// 기본값 설정. ActorsUseNameArea, ObjectsUserNameArea, HideNameArea
		FDetailsViewArgs args(false, false, true, FDetailsViewArgs::HideNameArea);
		/* 디테일 뷰가 어려개가 나오기 때문에 식별자를 주는 것이다.
		나중에 게임에 접근하는 경우가 생기면 이 이름으로 찾게 된다.*/
		args.ViewIdentifier = "WeaponAssetEditorDetailsView";
		DetailsView = prop.CreateDetailView(args);

		FOnGetDetailCustomizationInstance detailView;
		detailView.BindStatic(&SWeaponDetailsView::MakeInstance);
		DetailsView->SetGenericLayoutDetailsDelegate(detailView);
	}
	
	//EquipmentData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponEquipmentData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("EquipmentData", instance);
	}

	//DoActionData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponDoActionData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("DoActionData", instance);
	}

	//HitData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponHitData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("HitData", instance);
	}

	//AirActionData
	{
		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponHitData::MakeInstance);
		prop.RegisterCustomPropertyTypeLayout("AirActionData", instance);
	}


	// 여기서부터 밑에가 디자인되는 부분이니까 이 전에 객체를 생성을 해주어야 할 것이다.
	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.4f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.175f) // 왼쪽을 17.5%
					->AddTab(LeftAreaTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.725f)
					->AddTab(DetailTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
			)
		);

	UCWeaponAsset* asset = nullptr;
	// 이게 아니라 이제 우리가 선택한 에셋으로 열리게 해주면 된다.
	//asset = LeftArea->GetFirstDataPtr()->Asset;

	if(InAssetName.Len() > 0)
	{
		FWeaponRowDataPtr ptr = LeftArea->GetRowDataPtrByName(InAssetName);

		if (LeftArea->SelectedRowDataPtrName() == InAssetName)
			return;

		if (ptr.IsValid())
			asset = ptr->Asset;
		
	}

	if(asset == nullptr)
		asset = LeftArea->GetFirstDataPtr()->Asset;


	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), 
		EditorName, layout, true, true, asset);

	//DetailsView->SetObject(asset);
	LeftArea->SelectDataPtr(asset);
}

bool FWeaponAssetEditor::OnRequestClose()
{
	if (!!DetailsView)
	{
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& prop = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			prop.UnregisterCustomPropertyTypeLayout("EquipmentData");
			prop.UnregisterCustomPropertyTypeLayout("DoActionData");
			prop.UnregisterCustomPropertyTypeLayout("HitData");
			prop.UnregisterCustomPropertyTypeLayout("AirActionData");
		}
	}

	if (LeftArea.IsValid())
		LeftArea.Reset();

	if (DetailsView.IsValid())
		DetailsView.Reset();

	return true;
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab tab;
	tab.BindSP(this, &FWeaponAssetEditor::Spawn_LeftAreaTab);
	TabManager->RegisterTabSpawner(LeftAreaTabId, tab);

	FOnSpawnTab tab2;
	tab2.BindSP(this, &FWeaponAssetEditor::Spawn_DetailsViewTab);
	TabManager->RegisterTabSpawner(DetailTabId, tab2);
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_LeftAreaTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			LeftArea.ToSharedRef()
	];
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_DetailsViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
}

void FWeaponAssetEditor::OnListViewSelectedItem(FWeaponRowDataPtr InDataPtr)
{
	if (InDataPtr == nullptr)
		return;

	if (!!GetEditingObject())
		RemoveEditingObject(GetEditingObject());

	// 다시 현재 창에 편집해줄 객체를 등록해주었다.
	AddEditingObject(InDataPtr->Asset);
	// 창안에 Detail View도 바뀌게 해주었다.
	DetailsView->SetObject(InDataPtr->Asset);

}

FName FWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName;
}

FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();
}

FLinearColor FWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);
}

FReply FWeaponAssetEditor::OnClicked()
{
	GLog->Log("OnClicked!!");

	return FReply::Handled();
}
