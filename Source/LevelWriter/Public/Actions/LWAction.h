#pragma once

#include "CoreMinimal.h"
#include "Core/LWBaseObject.h"
#include "LWAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionFinished, class ULWAction*, Action, bool, bSuccess);

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class LEVELWRITER_API ULWAction : public ULWBaseObject
{
	GENERATED_BODY()

public:
	FOnActionFinished OnFinished;

	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "LevelWriter|Action")
	void Execute(AActor* Instigator);
	virtual void Execute_Implementation(AActor* Instigator) { FinishAction(true); }

	virtual void Abort();

	UFUNCTION(BlueprintCallable, Category = "LevelWriter|Action")
	void FinishAction(bool bSuccess);

protected:
	bool bIsActive = false;
};