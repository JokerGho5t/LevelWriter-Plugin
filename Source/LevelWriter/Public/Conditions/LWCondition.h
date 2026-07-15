#pragma once

#include "CoreMinimal.h"
#include "Core/LWBaseObject.h"
#include "LWCondition.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class LEVELWRITER_API ULWCondition : public ULWBaseObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "LevelWriter|Condition")
	bool CheckCondition(AActor* Instigator);
	virtual bool CheckCondition_Implementation(AActor* Instigator) { return true; }
};