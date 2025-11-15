// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSWeaponBase.h"
#include "ProjectXSCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"

AXSWeaponBase::AXSWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

	// Create weapon meshes
	WeaponMesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh1P"));
	WeaponMesh1P->SetOnlyOwnerSee(true);
	WeaponMesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh1P->CastShadow = false;
	RootComponent = WeaponMesh1P;

	WeaponMesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	WeaponMesh3P->SetupAttachment(WeaponMesh1P);
	WeaponMesh3P->SetOwnerNoSee(true);
	WeaponMesh3P->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Default values
	WeaponName = FText::FromString(TEXT("Base Weapon"));
	WeaponDescription = FText::FromString(TEXT("A basic weapon"));
	
	FireMode = EXSWeaponFireMode::Hitscan;
	BaseDamage = 20.0f;
	FireRate = 600.0f; // 600 RPM
	MaxRange = 10000.0f;
	ProjectileSpeed = 3000.0f;

	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	MaxReserveAmmo = 120;
	ReserveAmmo = MaxReserveAmmo;
	ReloadTime = 2.0f;
	bIsReloading = false;

	MuzzleSocketName = FName("Muzzle");

	OwningCharacter = nullptr;
}

void AXSWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize ammo
	CurrentAmmo = MaxAmmo;
	ReserveAmmo = MaxReserveAmmo;
}

void AXSWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AXSWeaponBase, CurrentAmmo);
	DOREPLIFETIME(AXSWeaponBase, ReserveAmmo);
	DOREPLIFETIME(AXSWeaponBase, bIsReloading);
}

void AXSWeaponBase::AttachToCharacter(AProjectXSCharacter* Character)
{
	if (!Character)
	{
		return;
	}

	OwningCharacter = Character;
	SetOwner(Character);

	// Attach 1P mesh to character's 1P mesh
	if (USkeletalMeshComponent* CharacterMesh1P = Character->GetFirstPersonMesh())
	{
		WeaponMesh1P->AttachToComponent(CharacterMesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GripPoint"));
	}

	// Attach 3P mesh to character's 3P mesh
	if (USkeletalMeshComponent* CharacterMesh3P = Character->GetMesh())
	{
		WeaponMesh3P->AttachToComponent(CharacterMesh3P, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GripPoint"));
	}
}

void AXSWeaponBase::DetachFromCharacter()
{
	WeaponMesh1P->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	WeaponMesh3P->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	
	OwningCharacter = nullptr;
	SetOwner(nullptr);
}

bool AXSWeaponBase::ConsumeAmmo(int32 Amount)
{
	if (CurrentAmmo >= Amount)
	{
		CurrentAmmo -= Amount;
		OnRep_CurrentAmmo();
		return true;
	}
	return false;
}

void AXSWeaponBase::StartReload()
{
	// Can't reload if already reloading or ammo is full or no reserve ammo
	if (bIsReloading || CurrentAmmo >= MaxAmmo || ReserveAmmo <= 0)
	{
		return;
	}

	bIsReloading = true;

	// Only server sets the timer
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AXSWeaponBase::FinishReload, ReloadTime, false);
	}

	PlayReloadEffects();
}

void AXSWeaponBase::FinishReload()
{
	if (!bIsReloading)
	{
		return;
	}

	// Calculate how much ammo to add
	int32 AmmoNeeded = MaxAmmo - CurrentAmmo;
	int32 AmmoToAdd = FMath::Min(AmmoNeeded, ReserveAmmo);

	CurrentAmmo += AmmoToAdd;
	ReserveAmmo -= AmmoToAdd;

	bIsReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);

	OnRep_CurrentAmmo();
	OnRep_ReserveAmmo();
}

void AXSWeaponBase::CancelReload()
{
	if (!bIsReloading)
	{
		return;
	}

	bIsReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

FVector AXSWeaponBase::GetMuzzleLocation() const
{
	if (WeaponMesh1P && WeaponMesh1P->DoesSocketExist(MuzzleSocketName))
	{
		return WeaponMesh1P->GetSocketLocation(MuzzleSocketName);
	}
	return GetActorLocation();
}

FTransform AXSWeaponBase::GetMuzzleTransform() const
{
	if (WeaponMesh1P && WeaponMesh1P->DoesSocketExist(MuzzleSocketName))
	{
		return WeaponMesh1P->GetSocketTransform(MuzzleSocketName);
	}
	return GetActorTransform();
}

void AXSWeaponBase::PlayFireEffects_Implementation(const FVector& MuzzleLocation, const FVector& HitLocation)
{
	// Override in Blueprint or subclasses to add muzzle flash, sounds, etc.
}

void AXSWeaponBase::PlayReloadEffects_Implementation()
{
	// Override in Blueprint or subclasses to add reload sounds, animations, etc.
	
	// Play reload animation on character if available
	if (OwningCharacter)
	{
		if (USkeletalMeshComponent* Mesh1P = OwningCharacter->GetFirstPersonMesh())
		{
			if (ReloadAnimation1P)
			{
				Mesh1P->GetAnimInstance()->Montage_Play(ReloadAnimation1P);
			}
		}

		if (USkeletalMeshComponent* Mesh3P = OwningCharacter->GetMesh())
		{
			if (ReloadAnimation3P)
			{
				Mesh3P->GetAnimInstance()->Montage_Play(ReloadAnimation3P);
			}
		}
	}
}

void AXSWeaponBase::OnRep_CurrentAmmo()
{
	// Update UI or other visual feedback
}

void AXSWeaponBase::OnRep_ReserveAmmo()
{
	// Update UI or other visual feedback
}
