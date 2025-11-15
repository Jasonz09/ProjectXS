// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XSGameplayAbility.h"
#include "XSAbility_AreaDamage.generated.h"

/**
 * Area Damage ability - Creates a damaging area effect
 * Can be used for grenades, explosions, or other AoE abilities
 */
UCLASS()
class PROJECTXS_API UXSAbility_AreaDamage : public UXSGameplayAbility
{
	GENERATED_BODY()

public:
	UXSAbility_AreaDamage();

	/** Damage dealt in the area */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Area Damage")
	float AreaDamage;

	/** Radius of the damage area */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Area Damage")
	float DamageRadius;

	/** Range at which the ability can be cast */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Area Damage")
	float CastRange;

	/** Time before the area damage is applied (delay for grenade flight, etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Area Damage")
	float ActivationDelay;

	/** Whether damage falls off with distance from center */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Area Damage")
	bool bUseFalloff;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Determine target location for area damage */
	UFUNCTION(BlueprintCallable, Category = "Area Damage")
	FVector GetTargetLocation() const;

	/** Apply area damage */
	UFUNCTION(BlueprintCallable, Category = "Area Damage")
	void ApplyAreaDamage(const FVector& Location);

	/** Spawn visual effects */
	UFUNCTION(BlueprintNativeEvent, Category = "Area Damage")
	void SpawnEffects(const FVector& Location);

	FTimerHandle ActivationTimerHandle;
	FVector TargetLocation;
};
