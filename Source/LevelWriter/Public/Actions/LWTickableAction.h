#pragma once

#include "CoreMinimal.h"
#include "Actions/LWAction.h"
#include "Tickable.h"
#include "LWTickableAction.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class LEVELWRITER_API ULWTickableAction : public ULWAction, public FTickableGameObject
{
	GENERATED_BODY()

public:
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	virtual void Abort() override;

protected:
	
	virtual void OnTick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "LevelWriter|Action", meta = (DisplayName = "On Tick"))
	void ReceiveTick(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, Category = "LevelWriter|Action")
	bool bIsTicking = false;
};