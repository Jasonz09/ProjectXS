// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XSGameplayAbility.h"
#include "XSAbility_WeaponFire.generated.h"

/**
 * Base ability for weapon firing
 * Handles hitscan and projectile firing modes
 */
UCLASS()
class PROJECTXS_API UXSAbility_WeaponFire : public UXSGameplayAbility
{
	GENERATED_BODY()

public:
	UXSAbility_WeaponFire();

	/** Damage multiplier for this firing mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Fire")
	float DamageMultiplier;

	/** Whether to use spread */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Fire")
	bool bUseSpread;

	/** Max spread angle in degrees */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Fire", meta = (EditCondition = "bUseSpread"))
	float MaxSpreadAngle;

	/** Number of rays/projectiles to fire per shot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Fire")
	int32 ProjectilesPerShot;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	/** Fire weapon (server) */
	UFUNCTION(BlueprintCallable, Category = "Weapon Fire")
	virtual void FireWeapon();

	/** Perform hitscan trace */
	UFUNCTION(BlueprintCallable, Category = "Weapon Fire")
	virtual void PerformHitscan(const FVector& StartLocation, const FVector& Direction);

	/** Spawn projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon Fire")
	virtual void SpawnProjectile(const FVector& StartLocation, const FVector& Direction);

	/** Apply damage to hit actor */
	UFUNCTION(BlueprintCallable, Category = "Weapon Fire")
	virtual void ApplyDamage(AActor* HitActor, float Damage, const FVector& HitLocation);

	/** Get firing direction from camera */
	UFUNCTION(BlueprintPure, Category = "Weapon Fire")
	FVector GetFiringDirection() const;

	/** Get muzzle location */
	UFUNCTION(BlueprintPure, Category = "Weapon Fire")
	FVector GetMuzzleLocation() const;

	/** Apply spread to direction */
	FVector ApplySpread(const FVector& Direction) const;
};
