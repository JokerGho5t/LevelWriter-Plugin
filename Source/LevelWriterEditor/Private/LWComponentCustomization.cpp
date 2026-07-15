#include "LWComponentCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/Docking/TabManager.h"
#include "Styling/AppStyle.h"

TSharedRef<IDetailCustomization> FLWComponentCustomization::MakeInstance()
{
	return MakeShareable(new FLWComponentCustomization());
}

void FLWComponentCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("LevelWriter", FText::FromString("LevelWriter"), ECategoryPriority::Important);

	Category.AddCustomRow(FText::FromString("Open Inspector Button"))
		.WholeRowContent()
		[
			SNew(SBox)
			.Padding(FMargin(0.0f, 4.0f, 0.0f, 8.0f))
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.ContentPadding(FMargin(12.0f, 6.0f))
				.OnClicked(this, &FLWComponentCustomization::OnOpenInspectorClicked)
				.ToolTipText(FText::FromString("Open the custom LevelWriter Inspector panel for advanced event editing and debugging."))
				[
					SNew(STextBlock)
					.Text(FText::FromString("Open LevelWriter Inspector"))
					.Font(IDetailLayoutBuilder::GetDetailFontBold())
				]
			]
		];
}

FReply FLWComponentCustomization::OnOpenInspectorClicked()
{
	static const FName InspectorTabName("LevelWriterInspector");

	TSharedPtr<SDockTab> OpenedTab = FGlobalTabmanager::Get()->TryInvokeTab(InspectorTabName);

	if (!OpenedTab.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelWriter: Inspector tab is not registered yet! We will build it in Step 3."));
	}

	return FReply::Handled();
}