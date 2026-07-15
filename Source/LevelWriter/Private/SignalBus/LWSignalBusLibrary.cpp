#include "SignalBus/LWSignalBusLibrary.h"
#include "SignalBus/LWSignalBusSubsystem.h"
#include "Engine/World.h"

void ULWSignalBusLibrary::SendSignal(const UObject* WorldContextObject, FGameplayTag Channel, FGameplayTag SignalTag, UObject* Payload)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (ULWSignalBusSubsystem* Bus = World->GetSubsystem<ULWSignalBusSubsystem>())
		{
			Bus->BroadcastSignal(Channel, SignalTag, Payload);
		}
	}
}