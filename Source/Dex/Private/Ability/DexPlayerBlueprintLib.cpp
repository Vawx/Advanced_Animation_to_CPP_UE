
#include "Ability/DexPlayerBlueprintLib.h"
#include "Character/DexCharacterBase.h"

UDexPlayerBlueprintLib::UDexPlayerBlueprintLib(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{

}

bool UDexPlayerBlueprintLib::AttemptRoll(ADexCharacterBase* Base)
{
	if (Base)
	{
		return Base->Roll();
	}
	return false;
}
