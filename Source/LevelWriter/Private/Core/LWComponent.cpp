#include "Core/LWComponent.h"
#include "Core/LWEventScript.h"

#if WITH_EDITORONLY_DATA
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#endif

ULWComponent::ULWComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	EditorSprite = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("EditorSprite"));
	if (EditorSprite)
	{
		EditorSprite->SetupAttachment(this);
		
		static ConstructorHelpers::FObjectFinder<UTexture2D> SpriteTexture(TEXT("/LevelWriter/Icons/T_LWIcon"));
		if (SpriteTexture.Succeeded())
		{
			EditorSprite->Sprite = SpriteTexture.Object;
		}
		
		EditorSprite->bHiddenInGame = true;
		EditorSprite->bIsScreenSizeScaled = true;

		EditorSprite->SetDepthPriorityGroup(SDPG_Foreground);
		
		EditorSprite->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	}
#endif
}

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

void ULWComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
#if WITH_EDITORONLY_DATA
	if (EditorSprite)
	{
		EditorSprite->DestroyComponent();
		EditorSprite = nullptr;
	}
#endif

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void ULWComponent::AbortAllEvents()
{
	for (ULWEventScript* Event : Events)
	{
		if (Event) Event->AbortScript();
	}
}

#if WITH_EDITOR
void ULWComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ULWComponent, Events))
	{
		for (int32 i = 0; i < Events.Num(); ++i)
		{
			if (!Events[i])
			{
				Events[i] = NewObject<ULWEventScript>(this, ULWEventScript::StaticClass(), NAME_None, RF_Transactional);
			}
		}
	}
}
#endif