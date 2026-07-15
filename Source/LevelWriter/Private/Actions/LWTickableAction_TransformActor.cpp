#include "Actions/LWTickableAction_TransformActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Curves/CurveFloat.h"

void ULWTickableAction_TransformActor::Execute_Implementation(AActor* Instigator)
{
	CachedTarget = nullptr;
	if (TargetType == ELWTransformTarget::Instigator)
	{
		CachedTarget = Instigator;
	}
	else if (TargetType == ELWTransformTarget::ActorWithTag && TargetTag.IsValid())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, FoundActors);
		if (FoundActors.Num() > 0) CachedTarget = FoundActors[0];
	}

	if (!IsValid(CachedTarget))
	{
		FinishAction(false);
		return;
	}

	ActualStartTransform = bUseCurrentTransformAsStart ? CachedTarget->GetActorTransform() : StartTransform;
	
	ElapsedTime = 0.0f;
	
	bIsTicking = true; 

	if (!bWaitForCompletion)
	{
		OnFinished.Broadcast(this, true); 
	}
}

void ULWTickableAction_TransformActor::OnTick(float DeltaTime)
{
	if (!IsValid(CachedTarget))
	{
		bIsTicking = false;
		if (bWaitForCompletion) FinishAction(false);
		return;
	}

	ElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	if (EasingCurve) 
	{
		Alpha = EasingCurve->GetFloatValue(Alpha);
	}

	FTransform NewTransform = CachedTarget->GetActorTransform();
	
	if (bModifyLocation)
		NewTransform.SetLocation(FMath::Lerp(ActualStartTransform.GetLocation(), EndTransform.GetLocation(), Alpha));
	
	if (bModifyRotation)
		NewTransform.SetRotation(FQuat::Slerp(ActualStartTransform.GetRotation(), EndTransform.GetRotation(), Alpha));
	
	if (bModifyScale)
		NewTransform.SetScale3D(FMath::Lerp(ActualStartTransform.GetScale3D(), EndTransform.GetScale3D(), Alpha));

	CachedTarget->SetActorTransform(NewTransform);

	if (ElapsedTime >= Duration)
	{
		bIsTicking = false;
		
		if (bWaitForCompletion)
		{
			FinishAction(true);
		}
	}
}