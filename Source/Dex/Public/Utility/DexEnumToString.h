#pragma once

#include "CoreMinimal.h"

template<typename T>
class DEX_API DexEnumToString
{
public:

	static FString EnumToString(FString EnumName, T Value)
	{
		const FString DefaultValue = TEXT("Unable to figure out EnumName");
		UEnum* FoundEnum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		return FoundEnum ? FoundEnum->GetNameByIndex(static_cast<uint8>(Value)).ToString() : DefaultValue;
	}
};
