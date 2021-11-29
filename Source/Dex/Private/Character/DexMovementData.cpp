#include "Character/DexMovementData.h"

FComponentAndTransform::FComponentAndTransform(FTransform InTransform, UPrimitiveComponent* InComponent)
{
	Transform = InTransform;
	Component = MakeWeakObjectPtr<UPrimitiveComponent>(InComponent);
}

void FComponentAndTransform::Reset()
{
	Component.Reset();
	Transform = FTransform::Identity;
}

bool FComponentAndTransform::Valid()
{
	return (Component.Get(false) != nullptr);
}
