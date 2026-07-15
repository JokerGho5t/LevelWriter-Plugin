#include "Actions/LWTickableAction.h"

void ULWTickableAction::Tick(float DeltaTime)
{
	OnTick(DeltaTime);
	ReceiveTick(DeltaTime);
}

bool ULWTickableAction::IsTickable() const
{
	return bIsTicking && !HasAnyFlags(RF_ClassDefaultObject);
}

TStatId ULWTickableAction::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULWAction_Tickable, STATGROUP_Tickables);
}

void ULWTickableAction::Abort()
{
	bIsTicking = false;
	Super::Abort();
}

void ULWTickableAction::OnTick(float DeltaTime)
{
	
}
