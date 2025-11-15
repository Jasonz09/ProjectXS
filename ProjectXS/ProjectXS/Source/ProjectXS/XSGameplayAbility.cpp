// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSGameplayAbility.h"
#include "XSAbilityCharacter.h"
#include "XSWeaponBase.h"
#include "XSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UXSGameplayAbility::UXSGameplayAbility()
{
	// Default ability settings
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	AbilityName = FText::FromString(TEXT("Unnamed Ability"));
	AbilityDescription = FText::FromString(TEXT("No description"));

	EnergyCost = 0.0f;
	CooldownDuration = 0.0f;
	bCanActivateWhileMoving = true;
	bCanActivateInAir = true;
}

bool UXSGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, 
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// Check if we have enough energy
	if (!HasEnoughEnergy())
	{
		return false;
	}

	// Check movement restrictions
	if (AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo())
	{
		// Check if in air
		if (!bCanActivateInAir && Character->GetCharacterMovement() && Character->GetCharacterMovement()->IsFalling())
		{
			return false;
		}

		// Check if moving
		if (!bCanActivateWhileMoving && Character->GetVelocity().Size() > 0.0f)
		{
			return false;
		}
	}

	return true;
}

void UXSGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Consume energy on activation
	if (HasAuthority(&ActivationInfo))
	{
		ConsumeEnergy();
	}
}

void UXSGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AXSAbilityCharacter* UXSGameplayAbility::GetXSCharacterFromActorInfo() const
{
	return Cast<AXSAbilityCharacter>(GetAvatarActorFromActorInfo());
}

AXSWeaponBase* UXSGameplayAbility::GetWeaponFromActorInfo() const
{
	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	return Character ? Character->GetCurrentWeapon() : nullptr;
}

bool UXSGameplayAbility::HasEnoughEnergy() const
{
	if (AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo())
	{
		return Character->GetEnergy() >= EnergyCost;
	}
	return false;
}

void UXSGameplayAbility::ConsumeEnergy()
{
	if (EnergyCost > 0.0f)
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			if (const UXSAttributeSet* AttributeSet = ASC->GetSet<UXSAttributeSet>())
			{
				// Apply energy cost
				FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
				EffectContext.AddSourceObject(this);

				// Create a simple execution to modify energy
				ASC->ApplyModToAttribute(AttributeSet->GetEnergyAttribute(), EGameplayModOp::Additive, -EnergyCost);
			}
		}
	}
}
