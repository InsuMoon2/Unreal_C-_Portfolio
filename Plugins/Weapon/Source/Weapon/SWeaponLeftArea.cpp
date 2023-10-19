#include "SWeaponLeftArea.h"
#include "Weapon/CWeaponAsset.h"
#include "EngineUtils.h"
#include "Widgets/Input/SSearchBox.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable)
{
	// SLATE_ARGUMENT로 정의한걸 .으로 불러 올 수 있다.
	// 다른곳에서 부를때 ._으로 선언해놓으면 검색하였을때 그냥 버전과, _버전이 두개 검색된다
	// 그 중에 함수로 되어있는걸 사용하면 된다.
	Data = InArgs._RowData;

	// 어떤 형식으로 보여줄건지 스타일을 지정해줘야한다.
	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct
	(
		FSuperRowType::FArguments().Style(FEditorStyle::Get(), "TableView.DarkRow"), InOwnerTable
	);
}

TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	FString str;
	if (InColumnName == "Number")
		str = FString::FromInt(Data->Number);
	else if (InColumnName == "Name")
		str = Data->Name;

	return SNew(STextBlock)
		.Text(FText::FromString(str));

}

///////////////////////////////////////////////////////////////////////////////

void SWeaponLeftArea::Construct(const FArguments& InArgs)
{
	OnListViewSelectedItem = InArgs._OnSelectedItem;

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot() // 슬롯 추가
		.AutoHeight()
		.Padding(2, 0)
		[
			SAssignNew(SearchBox, SSearchBox)
			.SelectAllTextWhenFocused(true) // 검색창을 클릭하면 자동으로 텍스트가 선택되어있어도 다시 입력할 수 있게 해주는 기능이다.
			.OnTextChanged(this, &SWeaponLeftArea::OnTextChanged)
			.OnTextCommitted(this, &SWeaponLeftArea::OnTextCommitted)
		]


			+ SVerticalBox::Slot()
		.FillHeight(1) // 전체를 꽉 채우겠다. 1이 100%
		[
			// 이 구역안에 ListView를 할당해줄 것이다.
			SAssignNew(ListView, SListView <FWeaponRowDataPtr>)
			.HeaderRow // 머리 위에 출력해주는 HeaderRow (머릿말)
			// 칸칸히 구별되는 애들은 전부 다 Slot을 가지고 있다.
			(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Number")
				.DefaultLabel(FText::FromString("")) //언리얼의 위에 보여질 이름
				.ManualWidth(40)
				+ SHeaderRow::Column("Name")
				.DefaultLabel(FText::FromString("Name"))
			)
		// 리스트에서 어떤 자료형, 데터를 출력해 줄 것이냐
		// 리스트 뷰를 만들 때 자료형을 <FWeaponRowDataPtr>로 주었었다. 그러면
		// 얘에 대한 데이터를 주면 될 것이다. 만들어놓은 RowDatas를 넣어주면 된다.
		.ListItemsSource(&RowDatas)
		.OnGenerateRow(this, &SWeaponLeftArea::OnGenerateRow)
		.OnSelectionChanged(this, &SWeaponLeftArea::OnSelectionChanged)
		.SelectionMode(ESelectionMode::Single)
		]
		+ SVerticalBox::Slot() // Vertical Slot추가
			.AutoHeight()// 얘 딱 크기만큼만 높이가 만들어진다.
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			.Padding(FMargin(8, 2)) // 위 아래 8, 좌우 여백 2
			[
				SNew(STextBlock)
				.Text(this, &SWeaponLeftArea::OnGetAssetCount) // Attribute가 연결되어 있는 값을 자동 변경한다 했었다.
				]

		];

	ReadDataAssetList();
}

void SWeaponLeftArea::SelectDataPtr(UCWeaponAsset* InAsset)
{
	if (HasRowDataPtr() == false)
		return;

	for(FWeaponRowDataPtr ptr : RowDatas)
	{
		if(ptr->Asset == InAsset)
		{
			ListView->SetSelection(ptr);

			return;
		}
	}
}

FWeaponRowDataPtr SWeaponLeftArea::GetRowDataPtrByName(FString InAssetName)
{
	for(FWeaponRowDataPtr ptr : RowDatas)
	{
		if (ptr->Name == InAssetName)
			return ptr;
	}

	return nullptr;
}

FString SWeaponLeftArea::SelectedRowDataPtrName()
{
	TArray<FWeaponRowDataPtr> ptrs = ListView->GetSelectedItems();

	if (ptrs.Num() > 0)
		return ptrs[0]->Asset->GetName();

	return "";
}

TSharedRef<ITableRow> SWeaponLeftArea::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	return SNew(SWeaponTableRow, InTable)
		.RowData(InRow);
}

void SWeaponLeftArea::OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType)
{
	if (InDataPtr.IsValid() == false)
		return;

	OnListViewSelectedItem.ExecuteIfBound(InDataPtr);
}

FText SWeaponLeftArea::OnGetAssetCount() const
{
	FString str = FString::Printf(L"%d 에셋", RowDatas.Num());

	return FText::FromString(str);
}

void SWeaponLeftArea::OnTextChanged(const FText& InText)
{
	// 기존에 있는 문자와 같으면 할 필요가 없다.
	if (SearchText.CompareToCaseIgnored(InText) == 0)
		return;

	SearchText = InText;
	ReadDataAssetList(); // 재검색
}

void SWeaponLeftArea::OnTextCommitted(const FText& InText, ETextCommit::Type InType)
{
	OnTextChanged(InText);
}

void SWeaponLeftArea::ReadDataAssetList()
{
	// 호출될 때마다 비울 것이다.
	RowDatas.Empty();

	TArray<UObject*> objects;
	// 맨 앞에는 경로, 모든 에셋들을 다 찾아서 objects에 리턴, 클래스 타입이 아니라 일반 오브젝트라 Regular
	EngineUtils::FindOrLoadAssetsByPath("/Game/_Game/Weapon/", objects, EngineUtils::ATL_Regular);

	int32 index = 0;
	for (UObject* obj : objects)
	{
		UCWeaponAsset* asset = Cast<UCWeaponAsset>(obj);
		if (asset == nullptr) continue;

		FString name = asset->GetName();
		// SearchText가 비워져있지 않다면
		if (SearchText.IsEmpty() == false) 
		{
			// 에셋의 SearchText 문자열이 포함되어 있는지 확인하고, 없다면
			if (name.Contains(SearchText.ToString()) == false)
				continue;
		}

		// SearchText가 비워져 있다면
		RowDatas.Add(FWeaponRowData::Make(++index, name, asset));
	}

	RowDatas.Sort([](const FWeaponRowDataPtr& A, const FWeaponRowDataPtr& B)
		{
			return A->Number < B->Number;
		});

	ListView->RequestListRefresh();
}
