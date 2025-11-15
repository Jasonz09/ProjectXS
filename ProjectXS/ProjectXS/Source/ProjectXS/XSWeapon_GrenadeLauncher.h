// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XSWeaponBase.h"
#include "XSWeapon_GrenadeLauncher.generated.h"

/**
 * Grenade Launcher - Area damage projectile weapon
 * Ideal for Demolitionist role characters
 */
UCLASS()
class PROJECTXS_API AXSWeapon_GrenadeLauncher : public AXSWeaponBase
{
	GENERATED_BODY()

public:
	AXSWeapon_GrenadeLauncher();

	/** Explosion radius */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Grenade")
	float ExplosionRadius;

	/** Grenade arc multiplier (affects trajectory) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Grenade")
	float ArcMultiplier;
};
