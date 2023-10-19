#include "Widgets/CUserWidget_CrossHair.h"
#include "Global.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"

#define LOG_UCUserWidget_Crosshair 1

void UCUserWidget_CrossHair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* panel = Cast<UCanvasPanel>(WidgetTree->RootWidget);
	CheckNull(panel);

	TArray<UWidget*> widgets = panel->GetAllChildren();
	for (UWidget* widget : widgets)
	{
		UBorder* border = Cast<UBorder>(widget);
		if (border == nullptr) continue;

		Borders.Add(border);

		UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(border);
		Alignments.Add(slot->GetAlignment());
	}

	CrossHair = (GetWidgetFromName("CrossHair"));
	image = Cast<UImage>(CrossHair);

#if LOG_UCUserWidget_Crosshair
	for (int32 i = 0; i < Borders.Num(); i++)
	{
		UBorder* border = Borders[i];

		CLog::Log(border->GetName());
		CLog::Log(Alignments[i].ToString());
	}
#endif
}

void UCUserWidget_CrossHair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCUserWidget_CrossHair::CrossHair_Red()
{
	FSlateColor RedColor = FSlateColor(FLinearColor::Red);
	image->SetBrushTintColor(RedColor);
}

void UCUserWidget_CrossHair::CrossHair_White()
{
	FSlateColor WhiteColor = FSlateColor(FLinearColor::White);
	image->SetBrushTintColor(WhiteColor);
}
