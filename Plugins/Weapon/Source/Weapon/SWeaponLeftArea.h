#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STableRow.h"

struct FWeaponRowData
{
	int Number;
	FString Name;
	class UCWeaponAsset* Asset;

	FWeaponRowData()
	{
		
	}

	FWeaponRowData(int32 InNumber, FString InName, class UCWeaponAsset* InAsset)
		: Number(InNumber), Name(InName), Asset(InAsset)
	{
		
	}

	static TSharedPtr<FWeaponRowData> Make(int32 InNumber, FString InName, class UCWeaponAsset* InAsset)
	{
		return MakeShareable(new FWeaponRowData(InNumber, InName, InAsset));
	}
};
typedef TSharedPtr<FWeaponRowData> FWeaponRowDataPtr;

///////////////////////////////////////////////////////////////////////////////

class WEAPON_API SWeaponTableRow
// 실제로 출력할 자료형인 FWeaponRowDataPtr을 넣어주었다.
	: public SMultiColumnTableRow<FWeaponRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SWeaponTableRow) {};
	// 일반 변수를 줄 때에는 SLATE_ARGUMNET 사용, 앞에 자료형, 뒤에 변수명
	SLATE_ARGUMENT(FWeaponRowDataPtr, RowData)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

private:
	FWeaponRowDataPtr Data;
};

//////////////////////////////////////////////////////////////////////////////

DECLARE_DELEGATE_OneParam(FOnWeaponListViewSelectedItem, FWeaponRowDataPtr);

class WEAPON_API SWeaponLeftArea
	: public SCompoundWidget
{
public:
	// 사용할 때에 매크로 선언을 하지 않으면 링크 오류가 난다.
	SLATE_BEGIN_ARGS(SWeaponLeftArea) {};
	SLATE_EVENT(FOnWeaponListViewSelectedItem, OnSelectedItem)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

	// 하나라도 데이터가 있는지 체크
	bool HasRowDataPtr() { return RowDatas.Num() > 0; }
	// 첫번째 요소 체크
	FWeaponRowDataPtr GetFirstDataPtr() { return RowDatas[0]; }

	/* 선택될 에셋을 줄 것이다. 내부에서 에셋을 선택하게 만들고,
	 걔에 따라 디테일 탭이 다르게 보이게 해줄 것이다.*/
	void SelectDataPtr(class UCWeaponAsset* InAsset);

	FWeaponRowDataPtr GetRowDataPtrByName(FString InAssetName);
	FString SelectedRowDataPtrName();

private:
	// 얘가 한줄 한줄에 대한 모양을 만들어주는 것이다.
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);
	void OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType);

	// 지금 얘가 FText인데, 그러면 리턴형은 반드시 그거이다.
	FText OnGetAssetCount() const; //멤버 수정을 안할 것이기 때문에 const

	void OnTextChanged(const FText& InText); // 입력한 텍스트가 들어감
	// 엔터처럼 완전 입력이 완료되었을 때 호출되는 이벤트
	void OnTextCommitted(const FText& InText, ETextCommit::Type InType);

private:
	void ReadDataAssetList();

private:
	FOnWeaponListViewSelectedItem OnListViewSelectedItem;

private:
	TArray<FWeaponRowDataPtr> RowDatas;
	TSharedPtr<SListView<FWeaponRowDataPtr>> ListView;

private:
	
	TSharedPtr<class SSearchBox> SearchBox;
	FText SearchText; // 검색 문자열을 관리할 것들이다.
};

 