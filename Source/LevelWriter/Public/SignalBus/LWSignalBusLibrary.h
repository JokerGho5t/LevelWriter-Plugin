#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "LWSignalBusLibrary.generated.h"

UCLASS()
class LEVELWRITER_API ULWSignalBusLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "LevelWriter|SignalBus")
	static void SendSignal(const UObject* WorldContextObject, FGameplayTag Channel, FGameplayTag SignalTag, UObject* Payload = nullptr);
};