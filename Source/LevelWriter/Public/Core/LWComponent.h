#pragma once

#include "Components/ActorComponent.h"
#include "LWComponent.generated.h"

class ULWEventScript;

UCLASS(ClassGroup= "LevelWriter", DisplayName= "LevelWriter Component", meta=(BlueprintSpawnableComponent, PrioritizeCategories="LevelWriter"))
class LEVELWRITER_API ULWComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	ULWComponent();

	UPROPERTY(Instanced, EditAnywhere, Category = "LevelWriter")
	TArray<TObjectPtr<ULWEventScript>> Events;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "LevelWriter")
	void AbortAllEvents();
};