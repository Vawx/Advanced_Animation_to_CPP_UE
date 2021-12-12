#include "DexGlobals.h"

void FDexGameplayTags::AddGameplayTag(FGameplayTag& Out, const ANSICHAR* Name)
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	Out = Manager.AddNativeGameplayTag(FName(Name));
}

FDexGameplayTags::FDexGameplayTags()
{
	AddGameplayTag(CharacteActionJump, "Player.Character.Action.Jump");
	AddGameplayTag(CharacterActionRolling, "Player.Character.Action.Rolling");
	AddGameplayTag(CharacterActionFalling, "Player.Character.Action.Falling");
	AddGameplayTag(CharacterActionAiming, "Player.Character.Action.Aiming");
	AddGameplayTag(CharacterLocomotionWalking, "Player.Character.Locomotion.Walking");
	AddGameplayTag(CharacterLocomotionRunning, "Player.Character.Locomotion.Running");
	AddGameplayTag(CharacterLocomotionSprinting, "Player.Character.Locomotion.Sprinting");	
	AddGameplayTag(BaseWeaponTypePistol, "Player.Character.BaseWeaponType.Pistol");
	AddGameplayTag(BaseWeaponTypeRifle, "Player.Character.BaseWeaponType.Rifle");
	AddGameplayTag(BaseWeaponTypeShotgun, "Player.Character.BaseWeaponType.Shotgun");
	AddGameplayTag(BaseWeaponTypeTorch, "Player.Character.BaseWeaponType.Torch");
	AddGameplayTag(BaseWeaponTypeRocketLauncher, "Player.Character.BaseWeaponType.RocketLauncher");
	AddGameplayTag(BaseWeaponTypeGrenadeLauncher, "Player.Character.BaseWeaponType.GrenadeLauncher");
	AddGameplayTag(CharacterRotationmodeLooking, "Player.Character.RotationMode.Looking");
	AddGameplayTag(CharacterRotationmodeVelocity, "Player.Character.RotationMode.Velocity");
	AddGameplayTag(CharacterRotationmodeAiming, "Player.Character.RotationMode.Aiming");
	AddGameplayTag(CharacterStateTrigger, "Player.Character.StateTrigger");

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