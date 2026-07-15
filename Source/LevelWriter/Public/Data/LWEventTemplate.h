#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LWEventTemplate.generated.h"

UENUM(BlueprintType)
enum class ELWLogicOperand : uint8 { AND, OR };

UENUM(BlueprintType)
enum class ELWExecutionMode : uint8 { Sequential, Parallel };

UENUM(BlueprintType)
enum class ELWParallelMode : uint8 { ALL, ANY };

UCLASS(BlueprintType)
class LEVELWRITER_API ULWEventTemplate : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Triggers", meta=(ToolTip = "Triggers that will start this event"))
	TArray<TSubclassOf<class ULWTrigger>> TriggerClasses;

	UPROPERTY(EditAnywhere, Category = "Conditions", meta=(ToolTip = "AND - ALL conditions must be met\nOR - AT LEAST ONE"))
	ELWLogicOperand ConditionOperand = ELWLogicOperand::AND;

	UPROPERTY(EditAnywhere, Category = "Conditions", meta=(ToolTip = "Conditions to check before starting actions"))
	TArray<TSubclassOf<class ULWCondition>> ConditionClasses;

	UPROPERTY(EditAnywhere, Category = "Template Setup", meta = (ToolTip = "If true, the script will run only once. To restart, you need call ResetDoOnce function"))
	bool bExecuteOnce = false;

	UPROPERTY(EditAnywhere, Category = "Execution", meta=(ToolTip = "Sequential - actions execute one by one\nParallel - all execute at once"))
	ELWExecutionMode ExecutionMode = ELWExecutionMode::Sequential;

	UPROPERTY(EditAnywhere, Category = "Execution", meta = (EditCondition = "ExecutionMode == ELWExecutionMode::Parallel", EditConditionHides, ToolTip = "ALL - waits for all actions to finish\nANY - finishes when at least one action finishes (interrupting others)"))
	ELWParallelMode ParallelMode = ELWParallelMode::ALL;

	UPROPERTY(EditAnywhere, Category = "Execution", meta=(ToolTip = "Number of times to repeat the actions\n-1 - infinite loop\n1 - single execution"))
	int32 LoopCount = 1;

	UPROPERTY(EditAnywhere, Category = "Actions", meta=(ToolTip = "Actions to execute"))
	TArray<TSubclassOf<class ULWAction>> ActionClasses;
};