#include "Actions/LWAction_SendSignal.h"
#include "SignalBus/LWSignalBusSubsystem.h"
#include "Engine/World.h"

void ULWAction_SendSignal::Execute_Implementation(AActor* Instigator)
{
	if (UWorld* World = GetWorld())
	{
		if (ULWSignalBusSubsystem* Bus = World->GetSubsystem<ULWSignalBusSubsystem>())
		{
			UObject* Payload = bPassInstigatorAsPayload ? Instigator : nullptr;
			Bus->BroadcastSignal(Channel, SignalTag, Payload);
		}
	}
	
	FinishAction(true);
}