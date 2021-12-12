#include "Ability/DexGameplayAbility.h"
#include "Ability/DexAbilitySystemComponent.h"

UDexGameplayAbility::UDexGameplayAbility()
{
	
}

bool UDexGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

bool UDexGameplayAbility::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* Payload) const
{
	return Super::ShouldAbilityRespondToEvent(ActorInfo, Payload);
}

bool UDexGameplayAbility::ShouldActivateAbility(ENetRole Role) const
{
	return Super::ShouldActivateAbility(Role);
}

void UDexGameplayAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UDexGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
}

void UDexGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UDexGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool UDexGameplayAbility::CanBeCanceled() const 
{
	return Super::CanBeCanceled();
}

void UDexGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (ActivationType == EDexGameplayAbilityActivationType::Passive)
	{
		FGameplayTagContainer ActorInfoOwnedTags;
		UAbilitySystemComponent* ActorComponent = ActorInfo->AbilitySystemComponent.Get(false);
		if (ActorComponent)
		{
			ActorComponent->GetOwnedGameplayTags(ActorInfoOwnedTags);
		}
		FGameplayTagContainer OutTags;
		FGameplayTagContainer Temp = FGameplayTagContainer();
		if (CanActivateAbility(Spec.Handle, ActorInfo, &ActorInfoOwnedTags, &Temp, &OutTags))
		{
			if (ActivationType == EDexGameplayAbilityActivationType::Passive)
			{
				FGameplayEventData Data;
				Data.Instigator = GetAvatarActorFromActorInfo();
				Data.InstigatorTags = ActorInfoOwnedTags;
				ActivateAbility(Spec.Handle, ActorInfo, Spec.ActivationInfo, &Data);
			}
		}
	}
}

void UDexGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UDexGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
}

void UDexGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UDexAbilitySystemComponent* Comp = GetAbilityComponentAs<UDexAbilitySystemComponent>())
	{
		Comp->AddLooseGameplayTags(AppliedTags);
	}
}

void UDexGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (UDexAbilitySystemComponent* Comp = GetAbilityComponentAs<UDexAbilitySystemComponent>())
	{
		Comp->RemoveLooseGameplayTags(AppliedTags);
	}
}

void UDexGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UDexGameplayAbility::ConfirmActivateSucceed()
{
	Super::ConfirmActivateSucceed();
}
