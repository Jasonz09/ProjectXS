// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XSGameplayAbility.h"
#include "XSAbility_Dash.generated.h"

/**
 * Dash ability - Quick burst of movement in a direction
 * Provides mobility and repositioning
 */
UCLASS()
class PROJECTXS_API UXSAbility_Dash : public UXSGameplayAbility
{
	GENERATED_BODY()

public:
	UXSAbility_Dash();

	/** Dash distance */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashDistance;

	/** Dash duration (how fast the dash happens) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashDuration;

	/** Whether to dash in movement direction or view direction */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	bool bDashInMovementDirection;

	/** Whether to grant invulnerability during dash */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	bool bGrantInvulnerability;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Perform the dash */
	UFUNCTION()
	void PerformDash();

	/** End the dash */
	UFUNCTION()
	void EndDash();

	FTimerHandle DashTimerHandle;
	FVector DashDirection;
	FVector StartLocation;
	FVector TargetLocation;
};
