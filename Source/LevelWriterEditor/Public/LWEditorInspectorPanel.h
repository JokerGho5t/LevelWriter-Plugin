#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/LWComponent.h"

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
	
	bool bIsLocked = false;

	TWeakObjectPtr<ULWComponent> SelectedComponent;

	TSharedPtr<SVerticalBox> MainContentBox;
};