#pragma once

#include "CoreMinimal.h"
#include "LWAction.h"
#include "LWAction_Delay.generated.h"

UCLASS(DisplayName = "Delay")
class LEVELWRITER_API ULWAction_Delay : public ULWAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Action")
	float Duration = 1.0f;

	virtual void Execute_Implementation(AActor* Instigator) override;
	virtual void Abort() override;
private:
	FTimerHandle TimerHandle;
	void OnDelayFinished();
};
