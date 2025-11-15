// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectXSCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "ActiveGameplayEffectHandle.h"
#include "XSAbilityCharacter.generated.h"

class UXSAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
class AXSWeaponBase;

/**
 * Character class with Gameplay Ability System integration
 * Extends ProjectXSCharacter with abilities, attributes, and unique weapon
 */
UCLASS(Abstract)
class PROJECTXS_API AXSAbilityCharacter : public AProjectXSCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXSAbilityCharacter();

	// ====== Components ======
	
	/** Ability System Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	/** Attribute Set */
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	const UXSAttributeSet* AttributeSet;

	// ====== IAbilitySystemInterface ======
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ====== Character Weapon ======
	
	/** Character's unique weapon class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Weapon")
	TSubclassOf<AXSWeaponBase> WeaponClass;

	/** Current weapon instance */
	UPROPERTY(BlueprintReadOnly, Category = "Character|Weapon", ReplicatedUsing = OnRep_CurrentWeapon)
	AXSWeaponBase* CurrentWeapon;

	/** Get current weapon */
	UFUNCTION(BlueprintPure, Category = "Character|Weapon")
	AXSWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

	// ====== Abilities ======
	
	/** Default abilities granted to this character on spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/** Passive ability granted automatically */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> PassiveAbility;

	/** Primary ability (usually weapon-based) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> PrimaryAbility;

	/** Secondary ability (character-specific utility) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> SecondaryAbility;

	/** Ultimate ability (high-impact, long cooldown) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> UltimateAbility;

	/** Handles for granted abilities */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	// ====== Effects ======
	
	/** Default effects applied on spawn (e.g., passive stats, regen) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	/** Handles for active effects */
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

	// ====== Character Info ======
	
	/** Character display name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Info")
	FText CharacterName;

	/** Character role/class description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Info")
	FText CharacterRole;

	/** Character description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Info")
	FText CharacterDescription;

	// ====== Methods ======

	/** Initialize ability system component */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void InitializeAbilitySystem();

	/** Grant default abilities to character */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void GiveDefaultAbilities();

	/** Apply default gameplay effects */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void ApplyDefaultEffects();

	/** Spawn and equip character's weapon */
	UFUNCTION(BlueprintCallable, Category = "Character|Weapon")
	virtual void SpawnAndEquipWeapon();

	/** Remove all abilities from character */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void RemoveAllAbilities();

	/** Try to activate ability by tag */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilityByTag(const FGameplayTag& AbilityTag);

	/** Get attribute value */
	UFUNCTION(BlueprintPure, Category = "Abilities")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	float GetEnergy() const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	float GetMaxEnergy() const;

	UFUNCTION(BlueprintPure, Category = "Abilities")
	float GetMoveSpeed() const;

	/** Check if character is alive */
	UFUNCTION(BlueprintPure, Category = "Character")
	bool IsAlive() const;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentWeapon();

	/** Called when the character dies */
	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	void OnDeath();

	virtual void OnDeath_Implementation();

private:
	/** Flag to ensure ability system is only initialized once */
	bool bAbilitySystemInitialized;
};
