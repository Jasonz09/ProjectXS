// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XSInteractionComponent.generated.h"

class AArtilActor;
class ADreamlandZone;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionStarted, AActor*, InteractableActor, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionCancelled);

/**
 * Component that handles player interactions with Artils
 * - Pickup Artils (Devils only)
 * - Plant Artils (hold E for 4 seconds)
 * - Defuse Artils (Guardians, hold E for 7 seconds)
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTXS_API UXSInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UXSInteractionComponent();

	// ====== SETTINGS ======
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange = 200.f; // 2 meters

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float InteractionCheckFrequency = 0.1f; // Check 10 times per second

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	FName InteractionKey = FName("Use"); // E key by default

	// ====== CURRENT STATE ======
	
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractable = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	bool bIsInteracting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	float InteractionProgress = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	float InteractionDuration = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	FString InteractionPrompt = "";

	// ====== EVENTS ======
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInteractionStarted OnInteractionStarted;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInteractionCompleted OnInteractionCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInteractionCancelled OnInteractionCancelled;

	// ====== PUBLIC FUNCTIONS ======
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void StartInteraction();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void StopInteraction();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CancelInteraction();

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetNearestInteractable() const;

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool CanInteract() const;

	UFUNCTION(BlueprintPure, Category = "Interaction")
	FString GetInteractionPromptText() const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Interaction detection
	void CheckForInteractables();
	bool IsInteractable(AActor* Actor) const;
	FString GetPromptForActor(AActor* Actor) const;

	// Interaction execution
	void UpdateInteractionProgress(float DeltaTime);
	void CompleteInteraction();

	// Artil specific
	void InteractWithArtil(AArtilActor* Artil);
	bool CanPickupArtil(AArtilActor* Artil) const;
	bool CanPlantArtil(AArtilActor* Artil) const;
	bool CanDefuseArtil(AArtilActor* Artil) const;

	FTimerHandle InteractionCheckTimer;
	AActor* InteractionTarget = nullptr;
};
