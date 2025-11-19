// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DreamlandZone.generated.h"

class UBoxComponent;
class UBillboardComponent;

USTRUCT(BlueprintType)
struct FPlantSpot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHiddenSpot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpotName = "Plant Spot";
};

/**
 * Dreamland Zone - Designated area on the map where Devils can plant Artils
 * - Defines valid planting locations
 * - Shows blue/pink outline to indicate zone boundaries
 * - Multiple zones per map (Zone A, Zone B, etc.)
 */
UCLASS()
class PROJECTXS_API ADreamlandZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADreamlandZone();

	// ====== COMPONENTS ======
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* ZoneVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* ZoneIcon;

	// ====== PROPERTIES ======
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FString ZoneName = "Dreamland A";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	FLinearColor ZoneOutlineColor = FLinearColor(0.5f, 0.2f, 1.0f, 0.8f); // Blue/Pink

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone|Plant Spots")
	TArray<FPlantSpot> PlantSpots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone|Plant Spots")
	float PlantSpotRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone")
	bool bShowDebugOutline = true;

	// ====== PUBLIC FUNCTIONS ======
	
	UFUNCTION(BlueprintCallable, Category = "Zone")
	bool IsLocationInZone(const FVector& Location) const;

	UFUNCTION(BlueprintCallable, Category = "Zone")
	bool IsLocationValidPlantSpot(const FVector& Location, float Tolerance = 150.f) const;

	UFUNCTION(BlueprintCallable, Category = "Zone")
	FVector GetNearestPlantSpot(const FVector& Location) const;

	UFUNCTION(BlueprintCallable, Category = "Zone")
	TArray<FVector> GetAllPlantSpotLocations() const;

	UFUNCTION(BlueprintCallable, Category = "Zone")
	void AddPlantSpotAtLocation(const FVector& WorldLocation, bool bIsHidden = false);

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void UpdatePlantSpotLocations();
	void DrawDebugOutline();
};
