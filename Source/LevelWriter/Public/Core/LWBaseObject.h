#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LWBaseObject.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class LEVELWRITER_API ULWBaseObject : public UObject
{
	GENERATED_BODY()
public:	
	virtual UWorld* GetWorld() const override;
};