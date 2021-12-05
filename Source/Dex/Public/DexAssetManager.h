
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AssetData.h"
#include "Engine/StreamableManager.h"
#include "DexGlobals.h"
#include "DexAssetManager.generated.h"

UCLASS(config = Game)
class DEX_API UDexAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UDexAssetManager& Get();
	static UDexAssetManager* GetIfValid();
	
	UDexAssetManager();

	virtual void PostInitProperties() override;
	virtual void LoadRedirectorMaps() override;
	virtual void StartInitialLoading() override;
	virtual void FinishInitialLoading() override;
	virtual void PostInitialAssetScan() override;
	virtual bool ShouldScanPrimaryAssetType(FPrimaryAssetTypeInfo& TypeInfo) const override;
	virtual bool DoesPrimaryAssetMatchCustomOverride(FPrimaryAssetId PrimaryAssetId, const FPrimaryAssetRulesCustomOverride& CustomOverride) const override;
};