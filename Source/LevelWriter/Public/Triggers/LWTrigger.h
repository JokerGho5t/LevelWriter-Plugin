#pragma once

#include "CoreMinimal.h"
#include "Core/LWBaseObject.h"
#include "LWTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLWTrigger, AActor*, Instigator);

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class LEVELWRITER_API ULWTrigger : public ULWBaseObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnLWTrigger OnTrigger;

	UFUNCTION(BlueprintCallable, Category = "LevelWriter|Trigger")
	void Trigger(AActor* Instigator) { OnTrigger.Broadcast(Instigator); }
	
	UFUNCTION(BlueprintNativeEvent, Category = "LevelWriter|Trigger")
	void BindTrigger();
	virtual void BindTrigger_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = "LevelWriter|Trigger")
	void UnbindTrigger();
	virtual void UnbindTrigger_Implementation() {}
};