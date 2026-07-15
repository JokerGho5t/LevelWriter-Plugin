#include "Core/LWBaseObject.h"

UWorld* ULWBaseObject::GetWorld() const
{	
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}