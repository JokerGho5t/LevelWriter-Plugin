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

	UPROPERTY(EditAnywhere, Category = "Execution Rules", meta = (ShowOnlyInnerProperties))
	FLWEventConfig Config;

	UPROPERTY(Instanced, EditAnywhere, meta=(ToolTip = "Triggers that will start this event"))
	TArray<TObjectPtr<ULWTrigger>> Triggers;

	UPROPERTY(Instanced, EditAnywhere, meta=(ToolTip = "Conditions to check before starting actions"))
	TArray<TObjectPtr<ULWCondition>> Conditions;


	UPROPERTY(Instanced, EditAnywhere, meta=(ToolTip = "Actions to execute"))
	TArray<TObjectPtr<ULWAction>> Actions;

	UFUNCTION(BlueprintCallable, Category = "LevelWriter|EventScript")
	void ResetDoOnce();

	void Initialize();
	void Deinitialize();

	UFUNCTION(BlueprintCallable, Category = "LevelWriter|EventScript")
	void AbortScript();

	UFUNCTION(BlueprintCallable, Category = "LevelWriter|EventScript")
	void ForceExecute(AActor* Instigator);

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