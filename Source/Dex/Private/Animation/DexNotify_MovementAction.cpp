

#include "Animation/DexNotify_MovementAction.h"
#include "Character/DexCharacterBase.h"
#include "Utility/DexEnumToString.h"

UDexNotify_MovementAction::UDexNotify_MovementAction(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UDexNotify_MovementAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (nullptr == MeshComp || nullptr == Animation)
	{
		return;
	}

	if (ADexCharacterBase* DexCharacter = Cast<ADexCharacterBase>(MeshComp->GetOwner()))
	{
		DexCharacter->MovementAction = MovementAction;
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration);	
}

void UDexNotify_MovementAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (nullptr == MeshComp || nullptr == Animation)
	{
		return;
	}
	
	if (ADexCharacterBase* DexCharacter = Cast<ADexCharacterBase>(MeshComp->GetOwner()))
	{
		if (DexCharacter->MovementAction == MovementAction)
		{
			DexCharacter->MovementAction = ECharacterMovementAction::None;
		}	
	}

	Super::NotifyEnd(MeshComp, Animation);	
}

FString UDexNotify_MovementAction::GetNotifyName_Implementation() const
{
	FString NotifyName = FString(TEXT("MovementAction: "));
	NotifyName.Append(DexEnumToString<ECharacterMovementAction>::EnumToString(FString(TEXT("EMovementAction")), MovementAction));
	return NotifyName;
}