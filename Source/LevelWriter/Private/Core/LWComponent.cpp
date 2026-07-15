#include "Core/LWComponent.h"
#include "Core/LWEventScript.h"

ULWComponent::ULWComponent() { PrimaryComponentTick.bCanEverTick = false; }

void ULWComponent::BeginPlay()
{
	Super::BeginPlay();
	for (ULWEventScript* Event : Events)
	{
		if (Event) Event->Initialize();
	}
}

void ULWComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AbortAllEvents();
	for (ULWEventScript* Event : Events)
	{
		if (Event) Event->Deinitialize();
	}
	Super::EndPlay(EndPlayReason);
}

void ULWComponent::AbortAllEvents()
{
	for (ULWEventScript* Event : Events)
	{
		if (Event) Event->AbortScript();
	}
}