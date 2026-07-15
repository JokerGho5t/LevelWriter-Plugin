#include "Actions/LWAction_Delay.h"

void ULWAction_Delay::Execute_Implementation(AActor* Instigator) {
	if (UWorld* World = GetWorld())
		World->GetTimerManager().SetTimer(TimerHandle, this, &ULWAction_Delay::OnDelayFinished, Duration, false);
}
void ULWAction_Delay::Abort() {
	if (UWorld* World = GetWorld()) World->GetTimerManager().ClearTimer(TimerHandle);
	Super::Abort();
}
void ULWAction_Delay::OnDelayFinished() { FinishAction(true); }