#pragma once

#include "CoreMinimal.h"
#include "LWAction.h"
#include "GameplayTagContainer.h"
#include "LWAction_SendSignal.generated.h"

UCLASS(DisplayName = "Send Signal")
class LEVELWRITER_API ULWAction_SendSignal : public ULWAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Signal")
	FGameplayTag Channel;

	UPROPERTY(EditAnywhere, Category = "Signal")
	FGameplayTag SignalTag;

	UPROPERTY(EditAnywhere, Category = "Signal", meta = (ToolTip = "If enabled, it will transfer the Instigator (the one who ran this script) how is the payload of the signal"))
	bool bPassInstigatorAsPayload = true;

	virtual void Execute_Implementation(AActor* Instigator) override;
};