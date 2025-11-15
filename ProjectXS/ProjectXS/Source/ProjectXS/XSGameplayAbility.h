// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "XSGameplayAbility.generated.h"

class AXSAbilityCharacter;
class AXSWeaponBase;

/**
 * Base class for all XS gameplay abilities
 * Provides common functionality and helper methods
 */
UCLASS(Abstract)
class PROJECTXS_API UXSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UXSGameplayAbility();

	/** Ability display name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText AbilityName;

	/** Ability description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText AbilityDescription;

	/** Energy cost to activate this ability */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Costs")
	float EnergyCost;

	/** Cooldown duration in seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Cooldown")
	float CooldownDuration;

	/** Whether this ability can be activated while moving */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	bool bCanActivateWhileMoving;

	/** Whether this ability can be activated in air */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	bool bCanActivateInAir;

	// ====== Helper Methods ======

	/** Get the owning XS character */
	UFUNCTION(BlueprintPure, Category = "Ability")
	AXSAbilityCharacter* GetXSCharacterFromActorInfo() const;

	/** Get the character's current weapon */
	UFUNCTION(BlueprintPure, Category = "Ability")
	AXSWeaponBase* GetWeaponFromActorInfo() const;

	/** Check if character has enough energy */
	UFUNCTION(BlueprintPure, Category = "Ability")
	bool HasEnoughEnergy() const;

	/** Consume energy for ability activation */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ConsumeEnergy();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
