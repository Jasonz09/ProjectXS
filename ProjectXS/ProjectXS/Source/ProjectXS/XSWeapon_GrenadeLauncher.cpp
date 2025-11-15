// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSWeapon_GrenadeLauncher.h"

AXSWeapon_GrenadeLauncher::AXSWeapon_GrenadeLauncher()
{
	// Weapon info
	WeaponName = FText::FromString(TEXT("Grenade Launcher"));
	WeaponDescription = FText::FromString(TEXT("Explosive projectile launcher. Deals area damage on impact."));

	// Weapon type
	FireMode = EXSWeaponFireMode::Projectile;

	// Damage and fire rate
	BaseDamage = 80.0f; // High damage for direct hit
	FireRate = 120.0f; // 120 RPM (slow fire rate)
	MaxRange = 8000.0f;
	ProjectileSpeed = 1500.0f; // Arc projectile

	// Grenade properties
	ExplosionRadius = 400.0f;
	ArcMultiplier = 1.2f;

	// Ammo
	MaxAmmo = 6;
	CurrentAmmo = MaxAmmo;
	MaxReserveAmmo = 18;
	ReserveAmmo = MaxReserveAmmo;
	ReloadTime = 2.5f; // Slower reload

	// Tags
	WeaponTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Type.Projectile")));
	WeaponTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Weapon.Launcher")));
}
