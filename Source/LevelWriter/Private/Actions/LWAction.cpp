#include "Actions/LWAction.h"

void ULWAction::StartAction(AActor* Instigator)
{
	bIsActive = true;
	Execute(Instigator);
}

void ULWAction::Abort()
{
	if (bIsActive)
	{
		bIsActive = false;
		FinishAction(false);
	}
}

void ULWAction::FinishAction(bool bSuccess)
{
	if (bIsActive)
	{
		bIsActive = false;
		OnFinished.Broadcast(this, bSuccess);
	}
}