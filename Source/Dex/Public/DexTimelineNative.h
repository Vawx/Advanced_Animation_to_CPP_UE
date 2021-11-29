#pragma once

#include "CoreMinimal.h"
#include "UObject/SoftObjectPtr.h"
#include "DexTimelineNative.generated.h"

class UCurveFloat;

DECLARE_DELEGATE_OneParam(FOnDexTimelineUpdate, const float /* CurveValueAtTime */)
DECLARE_DELEGATE(FOnDexTimelineFinished)

USTRUCT(BlueprintType)
struct FDexTimeline
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	FDexTimeline() : Time(5.f) { }
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Character Timeline")
	UCurveFloat* Curve;
	
	FOnDexTimelineUpdate OnCharacterTimelineUpdate;
	FOnDexTimelineFinished OnCharacterTimelineFinished;
	
	void Start(const AActor* InWorldContext, bool bForceStart);
	void Stop();	
	void Pause();
	void Restart();
	void Update();	
	bool Active();
	
	void SetPlayLength(const float Length);
	
	const float GetPlaybackValue();
	const UWorld* GetWorldRef() { return WorldContext.Get(false); }
	
protected:
	
	bool bActive;
	
private:
	
	float Time;
	FTimerHandle TimerHandle;
	
	TWeakObjectPtr<const UWorld> WorldContext;
};