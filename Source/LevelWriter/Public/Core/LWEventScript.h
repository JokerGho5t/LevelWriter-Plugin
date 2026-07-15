#pragma once

#include "CoreMinimal.h"
#include "LWBaseObject.h"
#include "Data/LWEventTemplate.h"
#include "LWEventScript.generated.h"

class ULWTrigger;
class ULWCondition;
class ULWAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEventFinished, class ULWEventScript*, Script, bool, bSuccess);

UCLASS(DefaultToInstanced, EditInlineNew, DisplayName = "Event Script", CollapseCategories)
class LEVELWRITER_API ULWEventScript : public ULWBaseObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "LevelWriter|Events")
	FOnEventFinished OnEventFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (DisplayThumbnail = "false", ToolTip = "Select a template to auto-fill the settings below. You can override them after selecting."))
	ULWEventTemplate* Template;

	UPROPERTY(EditAnywhere, Category = "Setup", meta = (DisplayName = "Save To Template (Click to Save)", ToolTip = "Saves the current settings to the selected template or create a new one"))
	bool bSaveToTemplate = false;

	UPROPERTY(Instanced, EditAnywhere, Category = "Triggers", meta=(ToolTip = "Triggers that will start this event"))
	TArray<TObjectPtr<ULWTrigger>> Triggers;
	
	UPROPERTY(EditAnywhere, Category = "Conditions", meta=(ToolTip = "AND - ALL conditions must be met\nOR - AT LEAST ONE"))
	ELWLogicOperand ConditionOperand = ELWLogicOperand::AND;

	UPROPERTY(Instanced, EditAnywhere, Category = "Conditions", meta=(ToolTip = "Conditions to check before starting actions"))
	TArray<TObjectPtr<ULWCondition>> Conditions;

	UPROPERTY(EditAnywhere, Category = "Execution", meta = (ToolTip = "If true, the script will run only once. To restart, you need call ResetDoOnce function"))
	bool bExecuteOnce = false;
	
	UPROPERTY(EditAnywhere, Category = "Execution", meta=(ToolTip = "Sequential - actions execute one by one\nParallel - all execute at once"))
	ELWExecutionMode ExecutionMode = ELWExecutionMode::Sequential;

	UPROPERTY(EditAnywhere, Category = "Execution", meta = (EditCondition = "ExecutionMode == ELWExecutionMode::Parallel", EditConditionHides, ToolTip = "ALL - waits for all actions to finish\nANY - finishes when at least one action finishes (interrupting others)"))
	ELWParallelMode ParallelMode = ELWParallelMode::ALL;

	UPROPERTY(EditAnywhere, Category = "Execution", meta=(ToolTip = "Number of times to repeat the actions\n-1 - infinite loop\n1 - single execution"))
	int32 LoopCount = 1;

	UPROPERTY(Instanced, EditAnywhere, Category = "Actions", meta=(ToolTip = "Actions to execute"))
	TArray<TObjectPtr<ULWAction>> Actions;

	UFUNCTION(BlueprintCallable, Category = "LevelWriter|EventScript")
	void ResetDoOnce();

	void Initialize();
	void Deinitialize();

	UFUNCTION(BlueprintCallable, Category = "LevelWriter|EventScript")
	void AbortScript(); 

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void ExecuteSaveToTemplate();
#endif

protected:
	void BindTriggers();
	void UnbindTriggers();
	bool CheckConditions(AActor* Instigator);

	UFUNCTION()
	void TryExecute(AActor* Instigator);

	void StartIteration();
	void ExecuteNextSequential();
	void CompleteIteration(bool bSuccess);
	void FinishScript(bool bSuccess);

	UFUNCTION()
	void OnActionFinished(ULWAction* Action, bool bSuccess);

private:
	bool bWasExecuted = false;
	bool bIsExecuting = false;
	int32 CurrentLoop = 0;
	int32 CurrentActionIdx = 0;
	int32 PendingParallelCount = 0;
	
	UPROPERTY()
	AActor* CurrentInstigator = nullptr;
};