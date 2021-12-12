#pragma once 

#include "CoreMinimal.h"
#include "GameplayTagsManager.h"
#include "GameplayTagContainer.h"

#include "DexGlobals.generated.h"

struct FDexGameplayTags
{	
	FGameplayTag CharacteActionJump;
	FGameplayTag CharacterActionRolling;
	FGameplayTag CharacterActionFalling;
	FGameplayTag CharacterActionAiming;
	FGameplayTag CharacterLocomotionWalking;
	FGameplayTag CharacterLocomotionRunning;
	FGameplayTag CharacterLocomotionSprinting;	
	FGameplayTag BaseWeaponTypePistol;
	FGameplayTag BaseWeaponTypeRifle;
	FGameplayTag BaseWeaponTypeShotgun;
	FGameplayTag BaseWeaponTypeTorch;
	FGameplayTag BaseWeaponTypeRocketLauncher;
	FGameplayTag BaseWeaponTypeGrenadeLauncher;
	FGameplayTag CharacterRotationmodeLooking;
	FGameplayTag CharacterRotationmodeVelocity;
	FGameplayTag CharacterRotationmodeAiming;
	FGameplayTag CharacterStateTrigger;

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
	FDexGameplayTags& GetGameplayTags() { return GameplayTags;  }
	static bool IsValid();
	

private:

	FDexGameplayTags GameplayTags;
};

