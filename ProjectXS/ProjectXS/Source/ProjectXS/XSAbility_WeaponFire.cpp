// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSAbility_WeaponFire.h"
#include "XSAbilityCharacter.h"
#include "XSWeaponBase.h"
#include "XSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UXSAbility_WeaponFire::UXSAbility_WeaponFire()
{
	AbilityName = FText::FromString(TEXT("Weapon Fire"));
	AbilityDescription = FText::FromString(TEXT("Fire the equipped weapon"));

	DamageMultiplier = 1.0f;
	bUseSpread = false;
	MaxSpreadAngle = 2.0f;
	ProjectilesPerShot = 1;
}

bool UXSAbility_WeaponFire::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, 
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// Check if we have a weapon
	AXSWeaponBase* Weapon = GetWeaponFromActorInfo();
	if (!Weapon)
	{
		return false;
	}

	// Check if weapon is reloading
	if (Weapon->bIsReloading)
	{
		return false;
	}

	// Check if we have ammo
	if (Weapon->CurrentAmmo < ProjectilesPerShot)
	{
		return false;
	}

	return true;
}

void UXSAbility_WeaponFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Fire weapon
	FireWeapon();

	// End ability immediately (instant fire)
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UXSAbility_WeaponFire::FireWeapon()
{
	AXSWeaponBase* Weapon = GetWeaponFromActorInfo();
	if (!Weapon)
	{
		return;
	}

	// Consume ammo
	if (!Weapon->ConsumeAmmo(ProjectilesPerShot))
	{
		return;
	}

	FVector MuzzleLocation = GetMuzzleLocation();
	FVector FiringDirection = GetFiringDirection();

	// Fire multiple projectiles/rays if needed
	for (int32 i = 0; i < ProjectilesPerShot; ++i)
	{
		FVector SpreadDirection = ApplySpread(FiringDirection);

		switch (Weapon->FireMode)
		{
		case EXSWeaponFireMode::Hitscan:
			PerformHitscan(MuzzleLocation, SpreadDirection);
			break;

		case EXSWeaponFireMode::Projectile:
			SpawnProjectile(MuzzleLocation, SpreadDirection);
			break;

		case EXSWeaponFireMode::Beam:
			// TODO: Implement beam weapon
			PerformHitscan(MuzzleLocation, SpreadDirection);
			break;
		}
	}

	// Play fire effects
	FVector EndLocation = MuzzleLocation + (FiringDirection * Weapon->MaxRange);
	Weapon->PlayFireEffects(MuzzleLocation, EndLocation);
}

void UXSAbility_WeaponFire::PerformHitscan(const FVector& StartLocation, const FVector& Direction)
{
	AXSWeaponBase* Weapon = GetWeaponFromActorInfo();
	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	
	if (!Weapon || !Character)
	{
		return;
	}

	FVector EndLocation = StartLocation + (Direction * Weapon->MaxRange);

	// Perform line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	QueryParams.AddIgnoredActor(Weapon);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		// Apply damage to hit actor
		float Damage = Weapon->BaseDamage * DamageMultiplier;
		ApplyDamage(HitResult.GetActor(), Damage, HitResult.ImpactPoint);

		// Debug draw
		#if !UE_BUILD_SHIPPING
		DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 1.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 8, FColor::Red, false, 2.0f);
		#endif
	}
	else
	{
		// Debug draw miss
		#if !UE_BUILD_SHIPPING
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 2.0f, 0, 1.0f);
		#endif
	}
}

void UXSAbility_WeaponFire::SpawnProjectile(const FVector& StartLocation, const FVector& Direction)
{
	AXSWeaponBase* Weapon = GetWeaponFromActorInfo();
	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	
	if (!Weapon || !Character || !Weapon->ProjectileClass)
	{
		return;
	}

	// Spawn projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character;
	SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator SpawnRotation = Direction.Rotation();
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(Weapon->ProjectileClass, StartLocation, SpawnRotation, SpawnParams);

	// TODO: Set projectile velocity, damage, etc.
	// This would typically be done in the projectile class itself
}

void UXSAbility_WeaponFire::ApplyDamage(AActor* HitActor, float Damage, const FVector& HitLocation)
{
	if (!HitActor)
	{
		return;
	}

	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	if (!Character)
	{
		return;
	}

	// Check if hit actor has an ability system component
	if (UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>())
	{
		// Apply damage through gameplay effect
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(Character);
		EffectContext.AddHitResult(FHitResult());

		// Create damage spec
		FGameplayEffectSpecHandle DamageSpec = TargetASC->MakeOutgoingSpec(nullptr, 1.0f, EffectContext);
		if (DamageSpec.IsValid())
		{
			// Manually apply damage to Damage attribute
			// (In production, use a GameplayEffect class)
			if (const UXSAttributeSet* TargetAttributes = TargetASC->GetSet<UXSAttributeSet>())
			{
				TargetASC->ApplyModToAttribute(TargetAttributes->GetDamageAttribute(), EGameplayModOp::Additive, Damage);
			}
		}
	}
	else
	{
		// Fallback to standard damage system
		UGameplayStatics::ApplyDamage(HitActor, Damage, Character->GetController(), Character, nullptr);
	}
}

FVector UXSAbility_WeaponFire::GetFiringDirection() const
{
	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	if (!Character)
	{
		return FVector::ForwardVector;
	}

	// Get direction from camera
	if (UCameraComponent* Camera = Character->GetFirstPersonCameraComponent())
	{
		return Camera->GetForwardVector();
	}

	return Character->GetActorForwardVector();
}

FVector UXSAbility_WeaponFire::GetMuzzleLocation() const
{
	AXSWeaponBase* Weapon = GetWeaponFromActorInfo();
	if (Weapon)
	{
		return Weapon->GetMuzzleLocation();
	}

	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	if (Character)
	{
		return Character->GetActorLocation();
	}

	return FVector::ZeroVector;
}

FVector UXSAbility_WeaponFire::ApplySpread(const FVector& Direction) const
{
	if (!bUseSpread || MaxSpreadAngle <= 0.0f)
	{
		return Direction;
	}

	// Random spread within cone
	float RandomAngle = FMath::FRandRange(0.0f, MaxSpreadAngle);
	float RandomRotation = FMath::FRandRange(0.0f, 360.0f);

	FRotator SpreadRotation = FRotator(
		FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * FMath::Cos(FMath::DegreesToRadians(RandomRotation)),
		FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * FMath::Sin(FMath::DegreesToRadians(RandomRotation)),
		0.0f
	);

	return (Direction.Rotation() + SpreadRotation).Vector();
}
