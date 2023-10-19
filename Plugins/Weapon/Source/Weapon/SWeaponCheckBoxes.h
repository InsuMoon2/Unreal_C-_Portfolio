#pragma once

#include "CoreMinimal.h"

class WEAPON_API SWeaponCheckBoxes
	: public TSharedFromThis<SWeaponCheckBoxes>
{
public:
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

	TSharedRef<SWidget> Draw(bool bBackground = false);
	void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildrenBuilder);


	void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);

private:
	void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);

public:
	static bool CanDraw(TSharedPtr<IPropertyHandle> InHandle, int InCount);

public:
	// Property 번호, 기본값
	void CheckDefaultObject(int32 InIndex, UObject* InValue);
	void CheckDefaultObject(int32 InIndex, USoundCue* InValue);
	void CheckDefaultObejct(int32 InIndex, TSubclassOf<class UMatineeCameraShake> CameraShakeClass);
	void CheckDefaultValue(int32 InIndex, float InValue);
	void CheckDefaultValue(int32 InIndex, bool InValue);
	void CheckDefaultValue(int32 InIndex, const FVector& InValue);
	void CheckDefaultValue(int32 InIndex, FVector& InValue);

private:
	struct FInternalData
	{
		bool bChecked; // 체크 되었느냐
		FString Name; // 이름
		TSharedPtr<IPropertyHandle> Handle; // 실제 식별자

		// 핸들을 하나 받음으로서 식별을 할 것이다.
		FInternalData(TSharedPtr<IPropertyHandle> InHandle)
		{
			bChecked = false;
			Handle = InHandle;

			// Handle 안에 실제로 출력될 이름인 DisplayName이 있다.
			Name = Handle->GetPropertyDisplayName().ToString();
		}
	};

	// 만든 구조체로 InternalDatas를 만들어주었음.
	TArray<FInternalData> InternalDatas;

	TSharedPtr<class IPropertyUtilities> Utilities;
};
