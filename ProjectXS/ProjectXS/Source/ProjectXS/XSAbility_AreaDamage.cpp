// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSAbility_AreaDamage.h"
#include "XSAbilityCharacter.h"
#include "XSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "TimerManager.h"

UXSAbility_AreaDamage::UXSAbility_AreaDamage()
{
	AbilityName = FText::FromString(TEXT("Area Damage"));
	AbilityDescription = FText::FromString(TEXT("Deploy an area damage effect."));

	EnergyCost = 40.0f;
	CooldownDuration = 8.0f;

	AreaDamage = 100.0f;
	DamageRadius = 500.0f;
	CastRange = 3000.0f;
	ActivationDelay = 1.0f;
	bUseFalloff = true;

	bCanActivateWhileMoving = true;
	bCanActivateInAir = false;

	// Set ability tags using SetAssetTags (UE 5.5+ API)
	FGameplayTagContainer Tags;
	Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.AreaDamage")));
	SetAssetTags(Tags);
}

void UXSAbility_AreaDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Get target location
	TargetLocation = GetTargetLocation();

	// Spawn projectile or indicator visuals
	SpawnEffects(TargetLocation);

	// Set timer for activation
	if (ActivationDelay > 0.0f)
	{
		FVector CapturedLocation = TargetLocation;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindWeakLambda(this, [this, CapturedLocation]()
		{
			ApplyAreaDamage(CapturedLocation);
		});
		GetWorld()->GetTimerManager().SetTimer(ActivationTimerHandle, TimerDelegate, ActivationDelay, false);
	}
	else
	{
		ApplyAreaDamage(TargetLocation);
	}

	// End ability (the damage will apply after delay)
	const float TotalDuration = ActivationDelay + 0.1f;
	FTimerHandle EndTimerHandle;
	FTimerDelegate EndDelegate;
	EndDelegate.BindUObject(this, &UXSAbility_AreaDamage::EndAbility, Handle, ActorInfo, ActivationInfo, true, false);
	GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, EndDelegate, TotalDuration, false);
}

FVector UXSAbility_AreaDamage::GetTargetLocation() const
{
	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	if (!Character)
	{
		return FVector::ZeroVector;
	}

	// Trace from camera forward
	FVector StartLocation = Character->GetActorLocation();
	FVector ForwardVector = Character->GetControlRotation().Vector();
	FVector EndLocation = StartLocation + (ForwardVector * CastRange);

	// Perform line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		return HitResult.ImpactPoint;
	}

	// If no hit, use max range
	return EndLocation;
}

void UXSAbility_AreaDamage::ApplyAreaDamage(const FVector& Location)
{
	AXSAbilityCharacter* Character = GetXSCharacterFromActorInfo();
	if (!Character)
	{
		return;
	}

	// Find all actors in radius
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);

	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Location,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(DamageRadius),
		QueryParams
	);

	// Apply damage to each hit actor
	for (const FOverlapResult& Overlap : OverlapResults)
	{
		AActor* HitActor = Overlap.GetActor();
		if (!HitActor)
		{
			continue;
		}

		// Calculate damage with falloff
		float FinalDamage = AreaDamage;
		if (bUseFalloff)
		{
			float Distance = FVector::Dist(Location, HitActor->GetActorLocation());
			float FalloffMultiplier = 1.0f - (Distance / DamageRadius);
			FinalDamage *= FMath::Max(0.0f, FalloffMultiplier);
		}

		// Apply damage
		if (UAbilitySystemComponent* TargetASC = HitActor->FindComponentByClass<UAbilitySystemComponent>())
		{
			// Apply damage through GAS
			if (const UXSAttributeSet* TargetAttributes = TargetASC->GetSet<UXSAttributeSet>())
			{
				TargetASC->ApplyModToAttribute(TargetAttributes->GetDamageAttribute(), EGameplayModOp::Additive, FinalDamage);
			}
		}
		else
		{
			// Fallback to standard damage
			UGameplayStatics::ApplyDamage(HitActor, FinalDamage, Character->GetController(), Character, nullptr);
		}
	}

	// Debug visualization
	#if !UE_BUILD_SHIPPING
	DrawDebugSphere(GetWorld(), Location, DamageRadius, 32, FColor::Orange, false, 2.0f, 0, 2.0f);
	#endif
}

void UXSAbility_AreaDamage::SpawnEffects_Implementation(const FVector& Location)
{
	// Override in Blueprint to spawn particle effects, sounds, etc.
	// This base implementation does nothing
	
	// Debug visualization
	#if !UE_BUILD_SHIPPING
	DrawDebugSphere(GetWorld(), Location, 50.0f, 16, FColor::Red, false, ActivationDelay, 0, 2.0f);
	DrawDebugLine(GetWorld(), GetXSCharacterFromActorInfo()->GetActorLocation(), Location, FColor::Yellow, false, ActivationDelay, 0, 1.0f);
	#endif
}
