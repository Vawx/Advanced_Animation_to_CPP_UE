#include "DexTimelineNative.h"

void FDexTimeline::Start(const AActor* InWorldContext, bool bForceStart)
{
	if (!WorldContext.IsValid(false, false))
	{
		WorldContext = InWorldContext->GetWorld();
	}
	
	if (const UWorld* World = GetWorldRef())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		if (TimerManager.IsTimerActive(TimerHandle) && !bForceStart)
		{
			return;
		}

		bActive = true;
		TimerManager.ClearTimer(TimerHandle);
		if (FMath::IsNearlyZero(Time))
		{
			Stop();
		}
		else
		{
			TimerManager.SetTimer(TimerHandle, FTimerDelegate::CreateRaw(this, &FDexTimeline::Stop), Time, false);
		}
	}	
}

void FDexTimeline::Update()
{
	if (const UWorld* World = GetWorldRef())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		if (TimerManager.IsTimerActive(TimerHandle))
		{
			const float CurrentValue = GetPlaybackValue();
			OnCharacterTimelineUpdate.ExecuteIfBound(CurrentValue);
		}
	}
}

bool FDexTimeline::Active()
{
	return bActive;
}

void FDexTimeline::Stop()
{
	if (bActive)
	{
		OnCharacterTimelineFinished.ExecuteIfBound();
		if (const UWorld* World = GetWorldRef())
		{
			FTimerManager& TimerManger = World->GetTimerManager();
			TimerManger.ClearTimer(TimerHandle);
			Time = 5.f;
			bActive = false;			
		}		
	}
}

void FDexTimeline::Pause()
{
	if (const UWorld* World = GetWorldRef())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.PauseTimer(TimerHandle);
	}
}

void FDexTimeline::Restart()
{
	if (!bActive)
	{
		AActor* WorldActor = Cast<AActor>(const_cast < UWorld* > (WorldContext.Get(false)));
		Start(WorldActor, true);
	}
}

void FDexTimeline::SetPlayLength(const float Length)
{
	Time = Length;
}

const float FDexTimeline::GetPlaybackValue()
{
	float Result = 0.f;
	if (UCurveFloat* C = Curve)
	{
		if (const UWorld* World = GetWorldRef())
		{
			FTimerManager& TimerManager = World->GetTimerManager();
			Result = C->GetFloatValue(TimerManager.GetTimerElapsed(TimerHandle));
		}
	}
	return Result;
}

