#include "SignalBus/LWAsyncAction_ListenSignal.h"
#include "Engine/World.h"

ULWAsyncAction_ListenSignal* ULWAsyncAction_ListenSignal::ListenForSignal(const UObject* WorldContextObject, FGameplayTag Channel, FGameplayTag SignalTag)
{
	ULWAsyncAction_ListenSignal* Node = NewObject<ULWAsyncAction_ListenSignal>();
	Node->WorldContext = WorldContextObject;
	Node->TargetChannel = Channel;
	Node->TargetSignal = SignalTag;
	return Node;
}

void ULWAsyncAction_ListenSignal::Activate()
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (ULWSignalBusSubsystem* Bus = World->GetSubsystem<ULWSignalBusSubsystem>())
		{
			Bus->OnSignalReceived.AddDynamic(this, &ULWAsyncAction_ListenSignal::HandleSignal);
		}
	}
}

void ULWAsyncAction_ListenSignal::HandleSignal(FGameplayTag ReceivedChannel, FGameplayTag ReceivedSignal, UObject* Payload)
{
	bool bChannelMatch = !TargetChannel.IsValid() || TargetChannel == ReceivedChannel;
	bool bSignalMatch = !TargetSignal.IsValid() || TargetSignal == ReceivedSignal;

	if (bChannelMatch && bSignalMatch)
	{
		OnSignal.Broadcast(ReceivedChannel, ReceivedSignal, Payload);
	}
}

void ULWAsyncAction_ListenSignal::SetReadyToDestroy()
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull))
	{
		if (ULWSignalBusSubsystem* Bus = World->GetSubsystem<ULWSignalBusSubsystem>())
		{
			Bus->OnSignalReceived.RemoveDynamic(this, &ULWAsyncAction_ListenSignal::HandleSignal);
		}
	}
	Super::SetReadyToDestroy();
}