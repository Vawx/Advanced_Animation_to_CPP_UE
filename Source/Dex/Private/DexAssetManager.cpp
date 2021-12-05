#include "DexAssetManager.h"

UDexAssetManager::UDexAssetManager()
{

}

UDexAssetManager& UDexAssetManager::Get()
{
	UDexAssetManager* Singleton = GetIfValid();	
	if (Singleton)
	{
		return *Singleton;
	}
	
	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager class used"));
	return *NewObject<UDexAssetManager>(); // never calls this
}

UDexAssetManager* UDexAssetManager::GetIfValid()
{
	if (GEngine && GEngine->AssetManager)
	{
		UDexAssetManager* Singleton = Cast<UDexAssetManager>(GEngine->AssetManager);		
		return Singleton;
	}	
	return nullptr;
}

void UDexAssetManager::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{

	}
}

void UDexAssetManager::LoadRedirectorMaps()
{
	Super::LoadRedirectorMaps();
}

void UDexAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UDexGlobals::Get();
}

void UDexAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();
}

void UDexAssetManager::PostInitialAssetScan()
{
	Super::PostInitialAssetScan();
}

bool UDexAssetManager::ShouldScanPrimaryAssetType(FPrimaryAssetTypeInfo& TypeInfo) const	
{
	return Super::ShouldScanPrimaryAssetType(TypeInfo);
}

bool UDexAssetManager::DoesPrimaryAssetMatchCustomOverride(FPrimaryAssetId PrimaryAssetId, const FPrimaryAssetRulesCustomOverride& CustomOverride) const
{
	return Super::DoesPrimaryAssetMatchCustomOverride(PrimaryAssetId,  CustomOverride);
}