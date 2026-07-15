#include "LevelWriterEditor.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "Core/LWComponent.h"
#include "LWComponentCustomization.h"
#include "LWEditorInspectorPanel.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FLevelWriterEditorModule"

static const FName LevelWriterTabName("LevelWriterInspector");

void FLevelWriterEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(
		ULWComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FLWComponentCustomization::MakeInstance)
	);
	PropertyModule.NotifyCustomizationModuleChanged();

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		LevelWriterTabName,
		FOnSpawnTab::CreateLambda([](const FSpawnTabArgs& Args) -> TSharedRef<SDockTab>
		{
			return SNew(SDockTab)
				.TabRole(ETabRole::NomadTab)
				[
					SNew(SLWEditorInspectorPanel)
				];
		}))
		.SetDisplayName(LOCTEXT("TabTitle", "LevelWriter Inspector"))
		.SetTooltipText(LOCTEXT("TooltipText", "Custom inspection panel for LevelWriter events"))
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory());
}

void FLevelWriterEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(ULWComponent::StaticClass()->GetFName());
	}

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(LevelWriterTabName);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLevelWriterEditorModule, LevelWriterEditor)