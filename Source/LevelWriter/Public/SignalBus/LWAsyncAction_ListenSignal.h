#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "LWSignalBusSubsystem.h"
#include "LWAsyncAction_ListenSignal.generated.h"

UCLASS()
class LEVELWRITER_API ULWAsyncAction_ListenSignal : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnLWSignalReceived OnSignal;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "LevelWriter|SignalBus")
	static ULWAsyncAction_ListenSignal* ListenForSignal(const UObject* WorldContextObject, FGameplayTag Channel, FGameplayTag SignalTag);

	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;

private:
	UFUNCTION()
	void HandleSignal(FGameplayTag ReceivedChannel, FGameplayTag ReceivedSignal, UObject* Payload);

	const UObject* WorldContext;
	FGameplayTag TargetChannel;
	FGameplayTag TargetSignal;
};