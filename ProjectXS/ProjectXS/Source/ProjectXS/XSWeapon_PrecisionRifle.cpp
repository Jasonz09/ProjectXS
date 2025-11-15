// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSWeapon_PrecisionRifle.h"

AXSWeapon_PrecisionRifle::AXSWeapon_PrecisionRifle()
{
	// Weapon info
	WeaponName = FText::FromString(TEXT("Precision Rifle"));
	WeaponDescription = FText::FromString(TEXT("High-accuracy hitscan rifle. Rewards precision with devastating headshots."));

	// Weapon type
	FireMode = EXSWeaponFireMode::Hitscan;

	// Damage and fire rate
	BaseDamage = 35.0f; // High damage per shot
	FireRate = 300.0f; // 300 RPM (semi-auto feel)
	MaxRange = 15000.0f; // Long range

	// Ammo
	MaxAmmo = 24;
	CurrentAmmo = MaxAmmo;
	MaxReserveAmmo = 96;
	ReserveAmmo = MaxReserveAmmo;
	ReloadTime = 1.8f; // Fast reload

	// Tags
	WeaponTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Type.Hitscan")));
	WeaponTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Rifle")));
}
