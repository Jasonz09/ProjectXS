// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSAbilityCharacter.h"
#include "AbilitySystemComponent.h"
#include "XSAttributeSet.h"
#include "XSWeaponBase.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

AXSAbilityCharacter::AXSAbilityCharacter()
{
	// Create ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create attribute set
	AttributeSet = CreateDefaultSubobject<UXSAttributeSet>(TEXT("AttributeSet"));

	bAbilitySystemInitialized = false;

	// Default character info
	CharacterName = FText::FromString(TEXT("Unknown Character"));
	CharacterRole = FText::FromString(TEXT("Unknown Role"));
	CharacterDescription = FText::FromString(TEXT("No description"));
}

void AXSAbilityCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize ability system on server and owning client
	if (HasAuthority())
	{
		InitializeAbilitySystem();
		SpawnAndEquipWeapon();
	}
}

void AXSAbilityCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server initializes ability system
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAbilitySystem();
	}
}

void AXSAbilityCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client initializes ability system
	if (AbilitySystemComponent && !bAbilitySystemInitialized)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAbilitySystem();
	}
}

void AXSAbilityCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AXSAbilityCharacter, CurrentWeapon);
}

UAbilitySystemComponent* AXSAbilityCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AXSAbilityCharacter::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent || bAbilitySystemInitialized)
	{
		return;
	}

	if (HasAuthority())
	{
		// Grant default abilities and effects
		GiveDefaultAbilities();
		ApplyDefaultEffects();
	}

	bAbilitySystemInitialized = true;
}

void AXSAbilityCharacter::GiveDefaultAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		return;
	}

	// Grant passive ability
	if (PassiveAbility)
	{
		FGameplayAbilitySpec Spec(PassiveAbility, 1, INDEX_NONE, this);
		FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
		GrantedAbilityHandles.Add(Handle);
		
		// Auto-activate passive ability
		AbilitySystemComponent->TryActivateAbility(Handle);
	}

	// Grant primary ability
	if (PrimaryAbility)
	{
		FGameplayAbilitySpec Spec(PrimaryAbility, 1, 0, this); // Input ID 0
		FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
		GrantedAbilityHandles.Add(Handle);
	}

	// Grant secondary ability
	if (SecondaryAbility)
	{
		FGameplayAbilitySpec Spec(SecondaryAbility, 1, 1, this); // Input ID 1
		FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
		GrantedAbilityHandles.Add(Handle);
	}

	// Grant ultimate ability
	if (UltimateAbility)
	{
		FGameplayAbilitySpec Spec(UltimateAbility, 1, 2, this); // Input ID 2
		FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
		GrantedAbilityHandles.Add(Handle);
	}

	// Grant other default abilities
	for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, this);
			FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
			GrantedAbilityHandles.Add(Handle);
		}
	}
}

void AXSAbilityCharacter::ApplyDefaultEffects()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& EffectClass : DefaultEffects)
	{
		if (EffectClass)
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1, EffectContext);
			if (SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				ActiveEffectHandles.Add(ActiveHandle);
			}
		}
	}
}

void AXSAbilityCharacter::SpawnAndEquipWeapon()
{
	if (!HasAuthority() || !WeaponClass)
	{
		return;
	}

	// Spawn weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	CurrentWeapon = GetWorld()->SpawnActor<AXSWeaponBase>(WeaponClass, SpawnParams);
	
	if (CurrentWeapon)
	{
		// Attach weapon to character
		CurrentWeapon->AttachToCharacter(this);

		// Grant weapon abilities
		if (AbilitySystemComponent)
		{
			if (CurrentWeapon->PrimaryFireAbility)
			{
				FGameplayAbilitySpec Spec(CurrentWeapon->PrimaryFireAbility, 1, 3, this); // Input ID 3
				FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
				GrantedAbilityHandles.Add(Handle);
			}

			if (CurrentWeapon->SecondaryFireAbility)
			{
				FGameplayAbilitySpec Spec(CurrentWeapon->SecondaryFireAbility, 1, 4, this); // Input ID 4
				FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
				GrantedAbilityHandles.Add(Handle);
			}

			if (CurrentWeapon->ReloadAbility)
			{
				FGameplayAbilitySpec Spec(CurrentWeapon->ReloadAbility, 1, 5, this); // Input ID 5
				FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
				GrantedAbilityHandles.Add(Handle);
			}
		}
	}
}

void AXSAbilityCharacter::RemoveAllAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (HasAuthority())
	{
		for (FGameplayAbilitySpecHandle& Handle : GrantedAbilityHandles)
		{
			AbilitySystemComponent->ClearAbility(Handle);
		}
		GrantedAbilityHandles.Empty();
	}
}

bool AXSAbilityCharacter::ActivateAbilityByTag(const FGameplayTag& AbilityTag)
{
	if (!AbilitySystemComponent)
	{
		return false;
	}

	return AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));
}

float AXSAbilityCharacter::GetHealth() const
{
	return AttributeSet ? AttributeSet->GetHealth() : 0.0f;
}

float AXSAbilityCharacter::GetMaxHealth() const
{
	return AttributeSet ? AttributeSet->GetMaxHealth() : 0.0f;
}

float AXSAbilityCharacter::GetEnergy() const
{
	return AttributeSet ? AttributeSet->GetEnergy() : 0.0f;
}

float AXSAbilityCharacter::GetMaxEnergy() const
{
	return AttributeSet ? AttributeSet->GetMaxEnergy() : 0.0f;
}

float AXSAbilityCharacter::GetMoveSpeed() const
{
	return AttributeSet ? AttributeSet->GetMoveSpeed() : 0.0f;
}

bool AXSAbilityCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AXSAbilityCharacter::OnRep_CurrentWeapon()
{
	// Update weapon on client when replicated
	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToCharacter(this);
	}
}

void AXSAbilityCharacter::OnDeath_Implementation()
{
	// Override in subclasses or Blueprint to handle death
	// Default: ragdoll, disable input, etc.
	
	if (AbilitySystemComponent)
	{
		// Cancel all active abilities
		AbilitySystemComponent->CancelAllAbilities();
	}

	// Disable movement
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->DisableMovement();
	}
}
