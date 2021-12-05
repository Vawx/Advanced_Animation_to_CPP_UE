#include "DexGlobals.h"

void FDexGameplayTags::AddGameplayTag(FGameplayTag& Out, const ANSICHAR* Name)
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	Out = Manager.AddNativeGameplayTag(FName(Name));
}

FDexGameplayTags::FDexGameplayTags()
{
	AddGameplayTag(DefaultGameplayTag, "Dex.Gameplay.Default.Tag");
	UGameplayTagsManager::Get().DoneAddingNativeTags();
}

UDexGlobals::UDexGlobals(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{


}

bool UDexGlobals::IsValid()
{	
	return GEngine && GEngine->GameSingleton != nullptr;
}

UDexGlobals& UDexGlobals::Get()
{
	UDexGlobals *Singleton = Cast<UDexGlobals>(GEngine->GameSingleton);	
	if (Singleton)
	{
		return *Singleton;
	}
	UE_LOG(LogTemp, Fatal, TEXT("Invalid Engine used"));
	return *NewObject<UDexGlobals>(); // never calls this
}