#include "LWEditorInspectorPanel.h"
#include "Selection.h"
#include "Editor.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "UObject/UnrealType.h" 
#include "UObject/UObjectGlobals.h"

void SLWEditorInspectorPanel::Construct(const FArguments& InArgs)
{
	USelection::SelectionChangedEvent.AddRaw(this, &SLWEditorInspectorPanel::OnEditorSelectionChanged);

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this, &SLWEditorInspectorPanel::OnPropertyChanged);

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(10.0f)
		[
			SAssignNew(MainContentBox, SVerticalBox)
		]
	];

	OnEditorSelectionChanged(nullptr);
}

SLWEditorInspectorPanel::~SLWEditorInspectorPanel()
{
	USelection::SelectionChangedEvent.RemoveAll(this);
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}

void SLWEditorInspectorPanel::OnEditorSelectionChanged(UObject* NewSelection)
{
	if (bIsLocked)
	{
		if (SelectedComponent.IsValid()) return;
		else bIsLocked = false;                 
	}
	
	SelectedComponent = nullptr;

	if (GEditor)
	{
		if (USelection* SelectedActors = GEditor->GetSelectedActors())
		{
			if (AActor* Actor = Cast<AActor>(SelectedActors->GetBottom<AActor>()))
			{
				SelectedComponent = Actor->FindComponentByClass<ULWComponent>();
			}
		}
	}

	RefreshUI();
}

void SLWEditorInspectorPanel::OnPropertyChanged(UObject* ObjectBeingModified,
	FPropertyChangedEvent& PropertyChangedEvent)
{
	if (!SelectedComponent.IsValid())
	{
		if (bIsLocked)
		{
			bIsLocked = false;
			RefreshUI();
		}
		return;
	}

	if (!ObjectBeingModified) return;

	if (ObjectBeingModified == SelectedComponent.Get() || ObjectBeingModified->IsIn(SelectedComponent.Get()))
	{
		RefreshUI();
	}
}

FReply SLWEditorInspectorPanel::OnToggleLockClicked()
{
	bIsLocked = !bIsLocked;
	
	RefreshUI();
	
	return FReply::Handled();
}

void SLWEditorInspectorPanel::RefreshUI()
{
	if (!MainContentBox.IsValid()) return;

	MainContentBox->ClearChildren();

	if (SelectedComponent.IsValid())
	{
		AActor* OwnerActor = SelectedComponent->GetOwner();
		int32 EventCount = SelectedComponent->Events.Num();

		FString LockButtonText = bIsLocked ? TEXT(" [ Locked ] ") : TEXT(" [ Lock Target ] ");
		FText LockTooltip = bIsLocked 
			? FText::FromString("Target is LOCKED. Click to unlock and allow selecting other actors.") 
			: FText::FromString("Click to LOCK this target. Selecting other actors in the viewport will not change this panel.");

		MainContentBox->AddSlot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 12.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("DetailsView.CategoryTop"))
				.Padding(FMargin(10.0f, 8.0f))
				[
					SNew(SHorizontalBox)
					
					// Левая часть: Имя Актора и счетчик событий
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("Selected Actor: %s  |  Attached Events: %d"), *OwnerActor->GetActorLabel(), EventCount)))
						.Font(FAppStyle::GetFontStyle("HeadingExtraSmall"))
						.ColorAndOpacity(FSlateColor::UseForeground())
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(8.0f, 0.0f, 0.0f, 0.0f)
					[
						SNew(SButton)
						.OnClicked(this, &SLWEditorInspectorPanel::OnToggleLockClicked)
						.ToolTipText(LockTooltip)
						.ContentPadding(FMargin(8.0f, 4.0f))
						[
							SNew(STextBlock)
							.Text(FText::FromString(LockButtonText))
							.Font(FAppStyle::GetFontStyle("NormalFontBold"))
							.ColorAndOpacity(bIsLocked ? FSlateColor(FLinearColor(1.0f, 0.8f, 0.0f)) : FSlateColor::UseForeground())
						]
					]
				]
			];

		MainContentBox->AddSlot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString("We will render clean event blocks and 'Force Call' debug buttons here in Step 4."))
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			];
	}
	else
	{
		bIsLocked = false;

		MainContentBox->AddSlot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillHeight(1.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Select an Actor with LevelWriter Component in the viewport to inspect events..."))
				.Font(FAppStyle::GetFontStyle("NormalFontBold"))
				.ColorAndOpacity(FSlateColor::UseSubduedForeground())
			];
	}
}
