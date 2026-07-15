#pragma once

#include "CoreMinimal.h"
#include "Actions/LWTickableAction.h"
#include "LWTickableAction_TransformActor.generated.h"

class UCurveFloat;

UENUM(BlueprintType)
enum class ELWTransformTarget : uint8
{
	Instigator,
	ActorWithTag
};

UCLASS(DisplayName = "Transform Actor")
class LEVELWRITER_API ULWTickableAction_TransformActor : public ULWTickableAction
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "Target")
	ELWTransformTarget TargetType = ELWTransformTarget::Instigator;

	UPROPERTY(EditAnywhere, Category = "Target", meta = (EditCondition = "TargetType == ELWTransformTarget::ActorWithTag", EditConditionHides))
	FName TargetTag;

	UPROPERTY(EditAnywhere, Category = "Transform", meta = (InlineEditConditionToggle))
	bool bModifyLocation = true;
	UPROPERTY(EditAnywhere, Category = "Transform", meta = (InlineEditConditionToggle))
	bool bModifyRotation = true;
	UPROPERTY(EditAnywhere, Category = "Transform", meta = (InlineEditConditionToggle))
	bool bModifyScale = true;

	UPROPERTY(EditAnywhere, Category = "Transform", meta = (ToolTip = "If true, the starting point will be the current coordinates of the actor at the time of launch"))
	bool bUseCurrentTransformAsStart = true;

	UPROPERTY(EditAnywhere, Category = "Transform", meta = (EditCondition = "!bUseCurrentTransformAsStart"))
	FTransform StartTransform;

	UPROPERTY(EditAnywhere, Category = "Transform")
	FTransform EndTransform;

	UPROPERTY(EditAnywhere, Category = "Execution", meta = (ClampMin = "0.001"))
	float Duration = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Execution", meta = (ToolTip = "Optional curve for smoothness (Easing). From 0 to 1"))
	UCurveFloat* EasingCurve;

	UPROPERTY(EditAnywhere, Category = "Execution", meta = (ToolTip = "If false, the action ends instantly for the EventScript, but the object continues to move in the background (Fire and Forget)"))
	bool bWaitForCompletion = true;

	virtual void Execute_Implementation(AActor* Instigator) override;

protected:
	virtual void OnTick(float DeltaTime) override;

private:
	UPROPERTY()
	AActor* CachedTarget = nullptr;

	FTransform ActualStartTransform;
	float ElapsedTime = 0.0f;
};