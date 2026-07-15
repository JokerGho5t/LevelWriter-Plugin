#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/LWComponent.h"

class IDetailsView;
class ULWEventScript;

class SLWEditorInspectorPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLWEditorInspectorPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SLWEditorInspectorPanel();

private:
	void OnEditorSelectionChanged(UObject* NewSelection);
	void OnPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent);
	void RefreshUI();
	FReply OnToggleLockClicked();

	FReply OnAddEventClicked();
	FReply OnDeleteEventClicked(ULWEventScript* ScriptToDelete);
	FReply OnForceCallClicked(ULWEventScript* Script);
	FReply OnAbortClicked(ULWEventScript* Script);

	void AppendEventCard(ULWEventScript* Script);

	TSharedRef<SWidget> CreateScriptDetailsView(ULWEventScript* Script);

	bool bIsLocked = false;
	TWeakObjectPtr<ULWComponent> SelectedComponent;
	
	TSharedPtr<SVerticalBox> MainContentBox;
	TSharedPtr<SVerticalBox> EventsContainerBox;
};