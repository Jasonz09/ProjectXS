// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtilActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EArtilState : uint8
{
	Dropped UMETA(DisplayName = "Dropped"),
	Carried UMETA(DisplayName = "Carried"),
	Planting UMETA(DisplayName = "Planting"),
	Planted UMETA(DisplayName = "Planted"),
	Defusing UMETA(DisplayName = "Defusing"),
	Defused UMETA(DisplayName = "Defused"),
	Detonated UMETA(DisplayName = "Detonated")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArtilStateChanged, EArtilState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArtilPlanted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArtilDefused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArtilDetonated);

/**
 * Artil - Spiky sphere explosive that Devils plant and Guardians defuse
 * - Can be picked up by Devils
 * - Drops on death
 * - Must be planted in Dreamland zones
 * - Detonates after 50 seconds unless defused
 */
UCLASS()
class PROJECTXS_API AArtilActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AArtilActor();

	// ====== COMPONENTS ======
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ArtilMesh;

	// ====== PROPERTIES ======
	
	UPROPERTY(ReplicatedUsing = OnRep_ArtilState, BlueprintReadOnly, Category = "Artil")
	EArtilState CurrentState = EArtilState::Dropped;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Artil")
	AActor* CurrentCarrier = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Artil|Timing")
	float PlantDuration = 4.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Artil|Timing")
	float DefuseDuration = 7.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Artil|Timing")
	float DetonationTime = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Artil|Explosion")
	float ExplosionRadius = 1500.f; // 15 meters

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Artil|Explosion")
	float ExplosionDamage = 200.f;

	UPROPERTY(BlueprintReadOnly, Category = "Artil")
	float PlantProgress = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Artil")
	float DefuseProgress = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Artil")
	float DetonationCountdown = 0.f;

	// ====== EVENTS ======
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnArtilStateChanged OnArtilStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnArtilPlanted OnPlanted;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnArtilDefused OnDefused;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnArtilDetonated OnDetonated;

	// ====== PUBLIC FUNCTIONS ======
	
	UFUNCTION(BlueprintCallable, Category = "Artil")
	bool CanBePickedUp(AActor* PickupActor) const;

	UFUNCTION(BlueprintCallable, Category = "Artil")
	void PickUp(AActor* NewCarrier);

	UFUNCTION(BlueprintCallable, Category = "Artil")
	void Drop(const FVector& DropLocation);

	UFUNCTION(BlueprintCallable, Category = "Artil")
	bool CanBePlanted(const FVector& PlantLocation) const;

	UFUNCTION(BlueprintCallable, Category = "Artil")
	void StartPlanting(AActor* Planter);

	UFUNCTION(BlueprintCallable, Category = "Artil")
	void InterruptPlanting();

	UFUNCTION(BlueprintCallable, Category = "Artil")
	void StartDefusing(AActor* Defuser);

	UFUNCTION(BlueprintCallable, Category = "Artil")
	void InterruptDefusing();

	UFUNCTION(BlueprintCallable, Category = "Artil")
	void Detonate();

	UFUNCTION(BlueprintPure, Category = "Artil")
	bool IsPlanted() const { return CurrentState == EArtilState::Planted; }

	UFUNCTION(BlueprintPure, Category = "Artil")
	bool IsCarried() const { return CurrentState == EArtilState::Carried; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ArtilState();

private:
	void ChangeState(EArtilState NewState);
	void UpdatePlantingProgress(float DeltaTime);
	void UpdateDefusingProgress(float DeltaTime);
	void UpdateDetonationCountdown(float DeltaTime);
	
	AActor* CurrentPlanter = nullptr;
	AActor* CurrentDefuser = nullptr;
};
