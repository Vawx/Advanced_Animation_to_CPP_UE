#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DexGameHUD.generated.h"

UCLASS()
class DEX_API ADexGameHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

public:

	void DrawAbilityTags();
	void ToggleDebugAbiltyTags();

protected:

	virtual void DrawHUD() override;
	bool bDrawAbilityTags;
	
};
