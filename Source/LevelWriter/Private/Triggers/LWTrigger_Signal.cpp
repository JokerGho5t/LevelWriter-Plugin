#include "Triggers/LWTrigger_Signal.h"
#include "SignalBus/LWSignalBusSubsystem.h"
#include "Engine/World.h"

void ULWTrigger_Signal::BindTrigger_Implementation()
{
	if (UWorld* World = GetWorld())
	{
		if (ULWSignalBusSubsystem* Bus = World->GetSubsystem<ULWSignalBusSubsystem>())
		{
			Bus->OnSignalReceived.AddDynamic(this, &ULWTrigger_Signal::OnSignalReceived);
		}
	}
}

void ULWTrigger_Signal::UnbindTrigger_Implementation()
{
	if (UWorld* World = GetWorld())
	{
		if (ULWSignalBusSubsystem* Bus = World->GetSubsystem<ULWSignalBusSubsystem>())
		{
			Bus->OnSignalReceived.RemoveDynamic(this, &ULWTrigger_Signal::OnSignalReceived);
		}
	}
}

void ULWTrigger_Signal::OnSignalReceived(FGameplayTag ReceivedChannel, FGameplayTag ReceivedSignal, UObject* Payload)
{
	bool bChannelMatch = !Channel.IsValid() || Channel == ReceivedChannel;
	bool bSignalMatch = !SignalTag.IsValid() || SignalTag == ReceivedSignal;

	if (bChannelMatch && bSignalMatch)
	{
		AActor* Instigator = Cast<AActor>(Payload);
		Trigger(Instigator); 
	}
}