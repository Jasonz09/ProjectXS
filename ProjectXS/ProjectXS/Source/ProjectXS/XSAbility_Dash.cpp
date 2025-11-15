// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSAbility_Dash.h"
#include "XSAbilityCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

UXSAbility_Dash::UXSAbility_Dash()
{
	AbilityName = FText::FromString(TEXT("Dash"));
	AbilityDescription = FText::FromString(TEXT("Quickly dash in a direction."));

	EnergyCost = 25.0f;
	CooldownDuration = 5.0f;
	
	DashDistance = 800.0f;
	DashDuration = 0.3f;
	bDashInMovementDirection = true;
	bGrantInvulnerability = false;

	bCanActivateWhileMoving = true;
	bCanActivateInAir = true;

	// Set ability tags using SetAssetTags (UE 5.5+ API)
	FGameplayTagContainer Tags;
	Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Dash")));
	SetAssetTags(Tags);
}

void UXSAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Determine dash direction
	if (bDashInMovementDirection)
	{
		// Use current movement direction
		FVector Velocity = Character->GetVelocity();
		if (Velocity.Size() > 0.0f)
		{
			DashDirection = Velocity.GetSafeNormal();
		}
		else
		{
			// No movement, use forward direction
			DashDirection = Character->GetActorForwardVector();
		}
	}
	else
	{
		// Use view direction
		DashDirection = Character->GetControlRotation().Vector();
	}

	// Calculate dash target location
	StartLocation = Character->GetActorLocation();
	TargetLocation = StartLocation + (DashDirection * DashDistance);

	// TODO: Add invulnerability if enabled
	// if (bGrantInvulnerability) { ... }

	// Perform dash using timeline or launch character
	if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
	{
		// Launch character in dash direction
		FVector LaunchVelocity = DashDirection * (DashDistance / DashDuration);
		Character->LaunchCharacter(LaunchVelocity, true, true);
	}

	// Set timer to end dash
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UXSAbility_Dash::EndDash, DashDuration, false);
}

void UXSAbility_Dash::PerformDash()
{
	// This could be used for timeline-based smooth dash
	// For now, we use LaunchCharacter for simplicity
}

void UXSAbility_Dash::EndDash()
{
	// Remove invulnerability if it was granted
	// Clear any dash effects

	const FGameplayAbilitySpecHandle Handle = GetCurrentAbilitySpecHandle();
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	const FGameplayAbilityActivationInfo ActivationInfo = GetCurrentActivationInfo();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
