#include "Core/LWEventScript.h"
#include "Triggers/LWTrigger.h"
#include "Conditions/LWCondition.h"
#include "Actions/LWAction.h"
#include "Data/LWEventTemplate.h"

void ULWEventScript::Initialize() { BindTriggers(); }
void ULWEventScript::Deinitialize() { AbortScript(); UnbindTriggers(); }

void ULWEventScript::BindTriggers()
{
	for (ULWTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->OnTrigger.AddDynamic(this, &ULWEventScript::TryExecute);
			Trigger->BindTrigger();
		}
	}
}

void ULWEventScript::UnbindTriggers()
{
	for (ULWTrigger* Trigger : Triggers)
	{
		if (Trigger)
		{
			Trigger->OnTrigger.RemoveDynamic(this, &ULWEventScript::TryExecute);
			Trigger->UnbindTrigger();
		}
	}
}

bool ULWEventScript::CheckConditions(AActor* Instigator)
{
	if (Conditions.IsEmpty()) return true;

	bool bResult = Conditions[0]->CheckCondition(Instigator);
	for (int32 i = 1; i < Conditions.Num(); ++i)
	{
		if (Config.ConditionOperand == ELWLogicOperand::AND)
			bResult &= Conditions[i]->CheckCondition(Instigator);
		else
			bResult |= Conditions[i]->CheckCondition(Instigator);
	}
	return bResult;
}

void ULWEventScript::TryExecute(AActor* Instigator)
{
	if (bIsExecuting || Actions.IsEmpty() || !CheckConditions(Instigator) || (Config.bExecuteOnce && bWasExecuted)) return;

	CurrentInstigator = Instigator;
	CurrentLoop = 0;
	bIsExecuting = true;
	StartIteration();
}

void ULWEventScript::StartIteration()
{
	if (Actions.IsEmpty()) { CompleteIteration(false); return; }

	if (Config.ExecutionMode == ELWExecutionMode::Parallel)
	{
		PendingParallelCount = Actions.Num();
		for (ULWAction* Action : Actions)
		{
			if (Action)
			{
				Action->OnFinished.AddDynamic(this, &ULWEventScript::OnActionFinished);
				Action->StartAction(CurrentInstigator);
			}
			else
			{
				PendingParallelCount--;
			}
		}
		
		if (PendingParallelCount <= 0) CompleteIteration(true);
	}
	else 
	{
		CurrentActionIdx = 0;
		ExecuteNextSequential();
	}
}

void ULWEventScript::ExecuteNextSequential()
{
	if (CurrentActionIdx < Actions.Num())
	{
		if (ULWAction* Action = Actions[CurrentActionIdx])
		{
			Action->OnFinished.AddDynamic(this, &ULWEventScript::OnActionFinished);
			Action->StartAction(CurrentInstigator);
		}
		else
		{
			CurrentActionIdx++;
			ExecuteNextSequential();
		}
	}
	else
	{
		CompleteIteration(true);
	}
}

void ULWEventScript::OnActionFinished(ULWAction* Action, bool bSuccess)
{
	Action->OnFinished.RemoveDynamic(this, &ULWEventScript::OnActionFinished);

	if (!bIsExecuting) return;

	if (Config.ExecutionMode == ELWExecutionMode::Parallel)
	{
		PendingParallelCount--;

		if (Config.ParallelMode == ELWParallelMode::ANY)
		{
			for (ULWAction* OtherAction : Actions)
			{
				if (OtherAction && OtherAction != Action)
				{
					OtherAction->OnFinished.RemoveDynamic(this, &ULWEventScript::OnActionFinished);
					OtherAction->Abort();
				}
			}
			CompleteIteration(bSuccess);
		}
		else if (PendingParallelCount <= 0) 
		{
			CompleteIteration(bSuccess); // All
		}
	}
	else 
	{
		if (bSuccess)
		{
			CurrentActionIdx++;
			ExecuteNextSequential();
		}
		else
		{
			CompleteIteration(false);
		}
	}
}

void ULWEventScript::CompleteIteration(bool bSuccess)
{
	if (!bIsExecuting) return;

	CurrentLoop++;
	
	if (bSuccess && (Config.LoopCount < 0 || CurrentLoop < Config.LoopCount))
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimerForNextTick(this, &ULWEventScript::StartIteration);
		}
		else
		{
			FinishScript(bSuccess);
		}
	}
	else
	{
		FinishScript(bSuccess);
	}
}

void ULWEventScript::FinishScript(bool bSuccess)
{
	if (bSuccess && Config.bExecuteOnce)
	{
		bWasExecuted = true;
	}
	
	bIsExecuting = false;
	OnEventFinished.Broadcast(this, bSuccess);
}

void ULWEventScript::AbortScript()
{
	if (!bIsExecuting) return;
	
	for (ULWAction* Action : Actions)
	{
		if (Action)
		{
			Action->OnFinished.RemoveDynamic(this, &ULWEventScript::OnActionFinished);
			Action->Abort();
		}
	}
	
	FinishScript(false);
}

void ULWEventScript::ResetDoOnce()
{
	bWasExecuted = false;
}

#if WITH_EDITOR
#include "Misc/MessageDialog.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Factories/DataAssetFactory.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"

void ULWEventScript::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ULWEventScript, bSaveToTemplate))
	{
		if (bSaveToTemplate)
		{
			bSaveToTemplate = false;
			ExecuteSaveToTemplate();
		}
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ULWEventScript, Template))
	{
		if (Template)
		{
			if (Template)
            		{
            			Config = Template->Config;
            
            			Triggers.Empty(); Conditions.Empty(); Actions.Empty();
            			for (auto& Cls : Template->TriggerClasses) if (Cls) Triggers.Add(NewObject<ULWTrigger>(this, Cls));
            			for (auto& Cls : Template->ConditionClasses) if (Cls) Conditions.Add(NewObject<ULWCondition>(this, Cls));
            			for (auto& Cls : Template->ActionClasses) if (Cls) Actions.Add(NewObject<ULWAction>(this, Cls));
            		}
		}
	}
}

void ULWEventScript::ExecuteSaveToTemplate()
{
	if (!Template)
	{
		EAppReturnType::Type CreateDecision = FMessageDialog::Open(
			EAppMsgType::YesNo, 
			FText::FromString(TEXT("The template is not selected. Create a new template asset for the current event?"))
		);

		if (CreateDecision != EAppReturnType::Yes) return;

		FSaveAssetDialogConfig SaveDialogConfig;
		SaveDialogConfig.DefaultPath = TEXT("/Game");
		SaveDialogConfig.DefaultAssetName = TEXT("Tpl_NewEventScript");
		SaveDialogConfig.DialogTitleOverride = FText::FromString(TEXT("Save a new event template"));

		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveDialogConfig);

		if (SaveObjectPath.IsEmpty()) return;

		FString PackagePath = FPackageName::GetLongPackagePath(SaveObjectPath);
		FString AssetName = FPaths::GetBaseFilename(SaveObjectPath);

		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
		Factory->DataAssetClass = ULWEventTemplate::StaticClass();

		UObject* NewAsset = AssetToolsModule.Get().CreateAsset(
			AssetName, 
			PackagePath, 
			ULWEventTemplate::StaticClass(), 
			Factory
		);

		Template = Cast<ULWEventTemplate>(NewAsset);
		if (!Template) return;
	}
	else
	{
		EAppReturnType::Type Decision = FMessageDialog::Open(
			EAppMsgType::YesNo, 
			FText::FromString(FString::Printf(TEXT("Overwrite the '%s' template?"), *Template->GetName()))
		);
		if (Decision != EAppReturnType::Yes) return;
	}

	Template->Config = Config;

    Template->TriggerClasses.Empty();
    Template->ConditionClasses.Empty();
    Template->ActionClasses.Empty();

	for (ULWTrigger* Trig : Triggers) if (Trig) Template->TriggerClasses.Add(Trig->GetClass());
	for (ULWCondition* Cond : Conditions) if (Cond) Template->ConditionClasses.Add(Cond->GetClass());
	for (ULWAction* Act : Actions) if (Act) Template->ActionClasses.Add(Act->GetClass());

	Template->Modify();
	Template->MarkPackageDirty();

	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("The template has been saved successfully!")));
}
#endif
