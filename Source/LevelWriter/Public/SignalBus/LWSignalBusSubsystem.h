#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "LWSignalBusSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLWSignalReceived, FGameplayTag, Channel, FGameplayTag, SignalTag, UObject*, Payload);

UCLASS()
class LEVELWRITER_API ULWSignalBusSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnLWSignalReceived OnSignalReceived;

	void BroadcastSignal(FGameplayTag Channel, FGameplayTag SignalTag, UObject* Payload)
	{
		OnSignalReceived.Broadcast(Channel, SignalTag, Payload);
	}
};