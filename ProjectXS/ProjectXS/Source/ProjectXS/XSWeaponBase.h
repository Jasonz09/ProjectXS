// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "XSWeaponBase.generated.h"

class AProjectXSCharacter;
class USkeletalMeshComponent;
class UGameplayAbility;
class UAnimMontage;

UENUM(BlueprintType)
enum class EXSWeaponFireMode : uint8
{
	Hitscan		UMETA(DisplayName = "Hitscan"),
	Projectile	UMETA(DisplayName = "Projectile"),
	Beam		UMETA(DisplayName = "Beam")
};

/**
 * Base weapon class for all character weapons
 * Weapons are tied to characters and grant abilities through the GAS
 */
UCLASS(Abstract, Blueprintable)
class PROJECTXS_API AXSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AXSWeaponBase();

	/** Weapon mesh (1P view) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh1P;

	/** Weapon mesh (3P view) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh3P;

	// ====== Weapon Properties ======
	
	/** Weapon display name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Info")
	FText WeaponName;

	/** Weapon description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Info")
	FText WeaponDescription;

	/** Fire mode (hitscan, projectile, beam) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Firing")
	EXSWeaponFireMode FireMode;

	/** Damage per shot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Firing")
	float BaseDamage;

	/** Fire rate (rounds per minute) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Firing")
	float FireRate;

	/** Max firing range */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Firing")
	float MaxRange;

	/** Projectile speed (if projectile mode) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Firing", meta = (EditCondition = "FireMode == EXSWeaponFireMode::Projectile"))
	float ProjectileSpeed;

	/** Projectile class to spawn (if projectile mode) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Firing", meta = (EditCondition = "FireMode == EXSWeaponFireMode::Projectile"))
	TSubclassOf<AActor> ProjectileClass;

	// ====== Ammo System ======
	
	/** Current ammo in magazine */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmo, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 CurrentAmmo;

	/** Max ammo per magazine */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 MaxAmmo;

	/** Reserve ammo */
	UPROPERTY(ReplicatedUsing = OnRep_ReserveAmmo, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 ReserveAmmo;

	/** Max reserve ammo */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 MaxReserveAmmo;

	/** Reload time in seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	float ReloadTime;

	/** Is weapon currently reloading */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon|Ammo")
	bool bIsReloading;

	// ====== Abilities ======
	
	/** Primary fire ability (granted to owner) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Abilities")
	TSubclassOf<UGameplayAbility> PrimaryFireAbility;

	/** Secondary fire ability (granted to owner) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Abilities")
	TSubclassOf<UGameplayAbility> SecondaryFireAbility;

	/** Reload ability (granted to owner) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Abilities")
	TSubclassOf<UGameplayAbility> ReloadAbility;

	/** Gameplay tags for this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Tags")
	FGameplayTagContainer WeaponTags;

	// ====== Animation ======
	
	/** Fire animation (1P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	UAnimMontage* FireAnimation1P;

	/** Fire animation (3P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	UAnimMontage* FireAnimation3P;

	/** Reload animation (1P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	UAnimMontage* ReloadAnimation1P;

	/** Reload animation (3P) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	UAnimMontage* ReloadAnimation3P;

	// ====== Methods ======

	/** Attach weapon to character */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void AttachToCharacter(AProjectXSCharacter* Character);

	/** Detach weapon from character */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void DetachFromCharacter();

	/** Get owning character */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	AProjectXSCharacter* GetOwningCharacter() const { return OwningCharacter; }

	/** Consume ammo (returns true if ammo was available) */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool ConsumeAmmo(int32 Amount = 1);

	/** Start reload */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StartReload();

	/** Finish reload */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void FinishReload();

	/** Cancel reload */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void CancelReload();

	/** Get muzzle socket location */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	FVector GetMuzzleLocation() const;

	/** Get muzzle socket transform */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	FTransform GetMuzzleTransform() const;

	/** Play fire effects */
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void PlayFireEffects(const FVector& MuzzleLocation, const FVector& HitLocation);

	/** Play reload effects */
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void PlayReloadEffects();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Owning character reference */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AProjectXSCharacter* OwningCharacter;

	/** Muzzle socket name */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName MuzzleSocketName;

	/** Timer handle for reload */
	FTimerHandle ReloadTimerHandle;

	UFUNCTION()
	void OnRep_CurrentAmmo();

	UFUNCTION()
	void OnRep_ReserveAmmo();
};
