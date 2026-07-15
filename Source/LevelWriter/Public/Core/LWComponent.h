#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LWComponent.generated.h"

class ULWEventScript;

UCLASS(ClassGroup= "LevelWriter", DisplayName= "LevelWriter Component", meta=(BlueprintSpawnableComponent, PrioritizeCategories="LevelWriter"))
class LEVELWRITER_API ULWComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	ULWComponent();

	UPROPERTY(Instanced, EditAnywhere, Category = "LevelWriter")
	TArray<TObjectPtr<ULWEventScript>> Events;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UFUNCTION(BlueprintCallable, Category = "LevelWriter")
	void AbortAllEvents();

#if WITH_EDITORONLY_DATA
	
	UPROPERTY()
	TObjectPtr<UBillboardComponent> EditorSprite;
#endif
};