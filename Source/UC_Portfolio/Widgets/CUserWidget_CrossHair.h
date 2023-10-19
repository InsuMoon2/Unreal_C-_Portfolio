#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeOnInitialized() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
		void CrossHair_Red();

	UFUNCTION()
		void CrossHair_White();

private:
	TArray<class UBorder*> Borders;
	TArray<FVector2D> Alignments;

private:
	class UWidget* CrossHair;
	class UImage* image;
};
