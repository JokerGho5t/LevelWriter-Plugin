#include "LWEditorInspectorPanel.h"
#include "Selection.h"
#include "Editor.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Styling/AppStyle.h"
#include "UObject/UnrealType.h"
#include "UObject/UObjectGlobals.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Modules/ModuleManager.h"
#include "Core/LWEventScript.h"

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
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SAssignNew(MainContentBox, SVerticalBox)
			]
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

void SLWEditorInspectorPanel::OnPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent)
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
		if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ULWComponent, Events))
		{
			if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd ||
				PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove ||
				PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear)
			{
				RefreshUI();
			}
		}
	}
}

FReply SLWEditorInspectorPanel::OnToggleLockClicked()
{
	bIsLocked = !bIsLocked;
	RefreshUI();
	return FReply::Handled();
}

FReply SLWEditorInspectorPanel::OnAddEventClicked()
{
	if (SelectedComponent.IsValid() && EventsContainerBox.IsValid())
	{
		SelectedComponent->Modify();
		ULWEventScript* NewScript = NewObject<ULWEventScript>(SelectedComponent.Get(), ULWEventScript::StaticClass(), NAME_None, RF_Transactional);
		SelectedComponent->Events.Add(NewScript);

		AppendEventCard(NewScript);
	}
	return FReply::Handled();
}

FReply SLWEditorInspectorPanel::OnDeleteEventClicked(ULWEventScript* ScriptToDelete)
{
	if (SelectedComponent.IsValid() && ScriptToDelete && EventsContainerBox.IsValid())
	{
		int32 IndexToRemove = SelectedComponent->Events.IndexOfByKey(ScriptToDelete);
		if (IndexToRemove != INDEX_NONE && EventsContainerBox->GetChildren()->Num() > IndexToRemove)
		{
			TSharedRef<SWidget> WidgetToRemove = EventsContainerBox->GetChildren()->GetChildAt(IndexToRemove);
			EventsContainerBox->RemoveSlot(WidgetToRemove);

			SelectedComponent->Modify();
			SelectedComponent->Events.RemoveAt(IndexToRemove);
		}
	}
	return FReply::Handled();
}

FReply SLWEditorInspectorPanel::OnForceCallClicked(ULWEventScript* Script)
{
	if (Script && SelectedComponent.IsValid())
	{
		AActor* OwnerActor = SelectedComponent->GetOwner();
		UE_LOG(LogTemp, Warning, TEXT("LevelWriter: Force executing event '%s' on actor '%s'..."), *Script->GetName(), *OwnerActor->GetActorLabel());
		Script->ForceExecute(OwnerActor);
	}
	return FReply::Handled();
}

FReply SLWEditorInspectorPanel::OnAbortClicked(ULWEventScript* Script)
{
	if (Script)
	{
		Script->AbortScript();
	}
	return FReply::Handled();
}

TSharedRef<SWidget> SLWEditorInspectorPanel::CreateScriptDetailsView(ULWEventScript* Script)
{
	if (!Script) return SNullWidget::NullWidget;

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.bShowOptions = false;

	TSharedPtr<IDetailsView> ScriptDetailsView = PropertyModule.CreateDetailView(DetailsViewArgs);
	ScriptDetailsView->SetObject(Script);

	return ScriptDetailsView.ToSharedRef();
}

void SLWEditorInspectorPanel::AppendEventCard(ULWEventScript* Script)
{
	if (!EventsContainerBox.IsValid() || !Script) return;

	TWeakObjectPtr<ULWEventScript> WeakScript = Script;
	TWeakObjectPtr<ULWComponent> WeakComp = SelectedComponent;

	EventsContainerBox->AddSlot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 16.0f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
			.Padding(8.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 0.0f, 0.0f, 8.0f)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("DetailsView.CategoryTop"))
					.BorderBackgroundColor(FLinearColor(0.05f, 0.05f, 0.08f, 1.0f))
					.Padding(FMargin(12.0f, 8.0f))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text_Lambda([WeakComp, WeakScript]() -> FText {
								if (!WeakComp.IsValid() || !WeakScript.IsValid()) return FText::FromString("Event [ Deleted ]");
								int32 DynamicIndex = WeakComp->Events.IndexOfByKey(WeakScript.Get());
								FString TplName = WeakScript->Template ? FString::Printf(TEXT("  [ Template: %s ]"), *WeakScript->Template->GetName()) : TEXT("");
								return FText::FromString(FString::Printf(TEXT("Event #%d%s"), DynamicIndex + 1, *TplName));
							})
							.Font(FAppStyle::GetFontStyle("HeadingExtraSmall"))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.85f, 0.3f, 1.0f)))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						.Padding(4.0f, 0.0f)
						[
							SNew(SButton)
							.OnClicked(this, &SLWEditorInspectorPanel::OnForceCallClicked, Script)
							.ToolTipText(FText::FromString("Force execute this event (bypasses triggers)."))
							.ContentPadding(FMargin(8.0f, 4.0f))
							[
								SNew(STextBlock)
								.Text(FText::FromString(" Force Call "))
								.Font(FAppStyle::GetFontStyle("NormalFontBold"))
								.ColorAndOpacity(FSlateColor(FLinearColor(0.3f, 1.0f, 0.3f)))
							]
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						.Padding(4.0f, 0.0f)
						[
							SNew(SButton)
							.OnClicked(this, &SLWEditorInspectorPanel::OnAbortClicked, Script)
							.ToolTipText(FText::FromString("Abort this event if running."))
							.ContentPadding(FMargin(8.0f, 4.0f))
							[
								SNew(STextBlock)
								.Text(FText::FromString(" Abort "))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.4f, 0.4f)))
							]
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						.Padding(12.0f, 0.0f, 0.0f, 0.0f)
						[
							SNew(SButton)
							.OnClicked(this, &SLWEditorInspectorPanel::OnDeleteEventClicked, Script)
							.ToolTipText(FText::FromString("Delete this event from the component."))
							.ContentPadding(FMargin(6.0f, 4.0f))
							[
								SNew(STextBlock)
								.Text(FText::FromString(" X "))
								.Font(FAppStyle::GetFontStyle("NormalFontBold"))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.2f, 0.2f)))
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					CreateScriptDetailsView(Script)
				]
			]
		];
}

void SLWEditorInspectorPanel::RefreshUI()
{
	if (!MainContentBox.IsValid()) return;

	MainContentBox->ClearChildren();

	if (SelectedComponent.IsValid())
	{
		AActor* OwnerActor = SelectedComponent->GetOwner();

		FString LockButtonText = bIsLocked ? TEXT(" [ Locked ] ") : TEXT(" [ Lock Target ] ");
		FText LockTooltip = bIsLocked 
			? FText::FromString("Target is LOCKED. Click to unlock.") 
			: FText::FromString("Click to LOCK this target.");

		MainContentBox->AddSlot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 12.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("DetailsView.CategoryTop"))
				.Padding(FMargin(10.0f, 8.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text_Lambda([this, OwnerActor]() -> FText {
							int32 Count = SelectedComponent.IsValid() ? SelectedComponent->Events.Num() : 0;
							return FText::FromString(FString::Printf(TEXT("Selected Actor: %s  |  Attached Events: %d"), *OwnerActor->GetActorLabel(), Count));
						})
						.Font(FAppStyle::GetFontStyle("HeadingExtraSmall"))
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
			.Padding(0.0f, 0.0f, 0.0f, 16.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &SLWEditorInspectorPanel::OnAddEventClicked)
					.ContentPadding(FMargin(12.0f, 6.0f))
					[
						SNew(STextBlock)
						.Text(FText::FromString(" + Add New Event "))
						.Font(FAppStyle::GetFontStyle("NormalFontBold"))
						.ColorAndOpacity(FSlateColor(FLinearColor(0.2f, 0.9f, 0.2f)))
					]
				]
			];

		MainContentBox->AddSlot()
			.AutoHeight()
			[
				SAssignNew(EventsContainerBox, SVerticalBox)
			];

		for (ULWEventScript* Script : SelectedComponent->Events)
		{
			AppendEventCard(Script);
		}
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