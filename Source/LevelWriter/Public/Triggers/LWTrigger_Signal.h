#pragma once

#include "CoreMinimal.h"
#include "LWTrigger.h"
#include "GameplayTagContainer.h"
#include "LWTrigger_Signal.generated.h"

UCLASS(DisplayName = "Signal Bus Trigger")
class LEVELWRITER_API ULWTrigger_Signal : public ULWTrigger
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Signal", meta = (ToolTip = "Канал для прослушивания. Если оставить пустым - слушает все каналы."))
	FGameplayTag Channel;

	UPROPERTY(EditAnywhere, Category = "Signal", meta = (ToolTip = "Тег сигнала. Если оставить пустым - слушает все сигналы в канале."))
	FGameplayTag SignalTag;

	virtual void BindTrigger_Implementation() override;
	virtual void UnbindTrigger_Implementation() override;

protected:
	UFUNCTION()
	void OnSignalReceived(FGameplayTag ReceivedChannel, FGameplayTag ReceivedSignal, UObject* Payload);
};