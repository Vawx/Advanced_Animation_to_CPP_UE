#include "CoreMinimal.h"
#include "GameplayTagsManager.h"
#include "GameplayTagContainer.h"

#include "DexGlobals.generated.h"

struct FDexGameplayTags
{	
	FGameplayTag DefaultGameplayTag;

	FDexGameplayTags();

private:

	inline void AddGameplayTag(FGameplayTag& Out, const ANSICHAR* Name);
};

UCLASS(config=Game)
class DEX_API UDexGlobals : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	static UDexGlobals& Get();
	static bool IsValid();
	

private:

	FDexGameplayTags GameplayTags;
};

