// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSInteractionComponent.h"
#include "ArtilActor.h"
#include "DreamlandZone.h"
#include "XSRoundBasedGameMode.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UXSInteractionComponent::UXSInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f; // Update 10 times per second
}

void UXSInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UXSInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check for nearby interactables
	CheckForInteractables();

	// Update interaction progress if interacting
	if (bIsInteracting)
	{
		UpdateInteractionProgress(DeltaTime);
	}
}

void UXSInteractionComponent::CheckForInteractables()
{
	AActor* PreviousInteractable = CurrentInteractable;
	CurrentInteractable = GetNearestInteractable();

	// Update prompt if interactable changed
	if (CurrentInteractable != PreviousInteractable)
	{
		if (CurrentInteractable)
		{
			InteractionPrompt = GetPromptForActor(CurrentInteractable);
		}
		else
		{
			InteractionPrompt = "";
		}
	}
}

AActor* UXSInteractionComponent::GetNearestInteractable() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return nullptr;

	FVector StartLocation = OwnerPawn->GetActorLocation();
	FVector ForwardVector = OwnerPawn->GetActorForwardVector();

	// Sphere trace for nearby actors
	TArray<FHitResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRange);
	
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		StartLocation + ForwardVector * InteractionRange,
		FQuat::Identity,
		ECC_Visibility,
		Sphere
	);

	if (!bHit)
		return nullptr;

	// Find closest interactable actor
	AActor* ClosestInteractable = nullptr;
	float ClosestDistance = InteractionRange;

	for (const FHitResult& Hit : HitResults)
	{
		if (Hit.GetActor() && IsInteractable(Hit.GetActor()))
		{
			float Distance = FVector::Dist(StartLocation, Hit.GetActor()->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestInteractable = Hit.GetActor();
			}
		}
	}

	return ClosestInteractable;
}

bool UXSInteractionComponent::IsInteractable(AActor* Actor) const
{
	if (!Actor)
		return false;

	// Check if it's an Artil
	AArtilActor* Artil = Cast<AArtilActor>(Actor);
	if (Artil)
	{
		return CanPickupArtil(Artil) || CanPlantArtil(Artil) || CanDefuseArtil(Artil);
	}

	return false;
}

FString UXSInteractionComponent::GetPromptForActor(AActor* Actor) const
{
	AArtilActor* Artil = Cast<AArtilActor>(Actor);
	if (!Artil)
		return "";

	if (CanPickupArtil(Artil))
		return "Press E to Pick Up Artil";
	
	if (CanPlantArtil(Artil))
		return "Hold E to Plant Artil";
	
	if (CanDefuseArtil(Artil))
		return "Hold E to Defuse Artil";

	return "";
}

bool UXSInteractionComponent::CanPickupArtil(AArtilActor* Artil) const
{
	if (!Artil || Artil->IsCarried() || Artil->IsPlanted())
		return false;

	// Only Devils can pickup Artils
	AXSRoundBasedGameMode* GameMode = Cast<AXSRoundBasedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return false;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return false;

	APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC)
		return false;

	return GameMode->GetPlayerTeam(PC) == ETeam::Devils;
}

bool UXSInteractionComponent::CanPlantArtil(AArtilActor* Artil) const
{
	if (!Artil || !Artil->IsCarried())
		return false;

	// Check if owner is carrying this Artil
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || Artil->CurrentCarrier != OwnerPawn)
		return false;

	// Check if in a Dreamland zone
	TArray<AActor*> FoundZones;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADreamlandZone::StaticClass(), FoundZones);

	for (AActor* ZoneActor : FoundZones)
	{
		ADreamlandZone* Zone = Cast<ADreamlandZone>(ZoneActor);
		if (Zone && Zone->IsLocationInZone(OwnerPawn->GetActorLocation()))
		{
			return true;
		}
	}

	return false;
}

bool UXSInteractionComponent::CanDefuseArtil(AArtilActor* Artil) const
{
	if (!Artil || !Artil->IsPlanted())
		return false;

	// Only Guardians can defuse
	AXSRoundBasedGameMode* GameMode = Cast<AXSRoundBasedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return false;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return false;

	APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC)
		return false;

	return GameMode->GetPlayerTeam(PC) == ETeam::Guardians;
}

bool UXSInteractionComponent::CanInteract() const
{
	return CurrentInteractable != nullptr && !bIsInteracting;
}

FString UXSInteractionComponent::GetInteractionPromptText() const
{
	return InteractionPrompt;
}

void UXSInteractionComponent::StartInteraction()
{
	if (!CanInteract())
		return;

	AArtilActor* Artil = Cast<AArtilActor>(CurrentInteractable);
	if (!Artil)
		return;

	InteractionTarget = CurrentInteractable;

	// Instant pickup
	if (CanPickupArtil(Artil))
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		Artil->PickUp(OwnerPawn);
		UE_LOG(LogTemp, Log, TEXT("Picked up Artil"));
		return;
	}

	// Start hold interaction
	bIsInteracting = true;
	InteractionProgress = 0.f;

	if (CanPlantArtil(Artil))
	{
		InteractionDuration = Artil->PlantDuration;
		Artil->StartPlanting(GetOwner());
		OnInteractionStarted.Broadcast(Artil, InteractionDuration);
		UE_LOG(LogTemp, Warning, TEXT("Started planting Artil"));
	}
	else if (CanDefuseArtil(Artil))
	{
		InteractionDuration = Artil->DefuseDuration;
		Artil->StartDefusing(GetOwner());
		OnInteractionStarted.Broadcast(Artil, InteractionDuration);
		UE_LOG(LogTemp, Warning, TEXT("Started defusing Artil"));
	}
}

void UXSInteractionComponent::StopInteraction()
{
	if (!bIsInteracting)
		return;

	// Only cancel if not complete
	if (InteractionProgress < InteractionDuration)
	{
		CancelInteraction();
	}
}

void UXSInteractionComponent::CancelInteraction()
{
	if (!bIsInteracting)
		return;

	bIsInteracting = false;
	InteractionProgress = 0.f;

	AArtilActor* Artil = Cast<AArtilActor>(InteractionTarget);
	if (Artil)
	{
		if (Artil->CurrentState == EArtilState::Planting)
		{
			Artil->InterruptPlanting();
		}
		else if (Artil->CurrentState == EArtilState::Defusing)
		{
			Artil->InterruptDefusing();
		}
	}

	OnInteractionCancelled.Broadcast();
	InteractionTarget = nullptr;
	
	UE_LOG(LogTemp, Warning, TEXT("Interaction cancelled"));
}

void UXSInteractionComponent::UpdateInteractionProgress(float DeltaTime)
{
	InteractionProgress += DeltaTime;

	// Check if player moved away or took damage
	AActor* CurrentNearby = GetNearestInteractable();
	if (CurrentNearby != InteractionTarget)
	{
		CancelInteraction();
		return;
	}

	// Check if completed
	if (InteractionProgress >= InteractionDuration)
	{
		CompleteInteraction();
	}
}

void UXSInteractionComponent::CompleteInteraction()
{
	bIsInteracting = false;
	OnInteractionCompleted.Broadcast();

	AArtilActor* Artil = Cast<AArtilActor>(InteractionTarget);
	if (Artil)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction completed!"));
		// Artil will handle its own state change
	}

	InteractionProgress = 0.f;
	InteractionTarget = nullptr;
}
