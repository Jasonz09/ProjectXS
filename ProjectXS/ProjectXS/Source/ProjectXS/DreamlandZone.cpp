// Copyright Epic Games, Inc. All Rights Reserved.

#include "DreamlandZone.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"

ADreamlandZone::ADreamlandZone()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create zone volume
	ZoneVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneVolume"));
	ZoneVolume->SetBoxExtent(FVector(1000.f, 1000.f, 300.f));
	ZoneVolume->SetCollisionProfileName(TEXT("OverlapAll"));
	ZoneVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZoneVolume->SetGenerateOverlapEvents(true);
	RootComponent = ZoneVolume;

	// Create icon for editor visibility
	ZoneIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("ZoneIcon"));
	ZoneIcon->SetupAttachment(RootComponent);
	ZoneIcon->SetRelativeLocation(FVector(0.f, 0.f, 400.f));

	// Default plant spots (can be edited in editor)
	PlantSpots.Add(FPlantSpot{FVector(0.f, 0.f, 0.f), false, "Center"});
	PlantSpots.Add(FPlantSpot{FVector(400.f, 0.f, 0.f), false, "Front"});
	PlantSpots.Add(FPlantSpot{FVector(-400.f, 0.f, 0.f), false, "Back"});
	PlantSpots.Add(FPlantSpot{FVector(0.f, 400.f, 0.f), false, "Right"});
	PlantSpots.Add(FPlantSpot{FVector(0.f, -400.f, 0.f), false, "Left"});
}

void ADreamlandZone::BeginPlay()
{
	Super::BeginPlay();

	UpdatePlantSpotLocations();

	if (bShowDebugOutline)
	{
		DrawDebugOutline();
	}
}

#if WITH_EDITOR
void ADreamlandZone::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdatePlantSpotLocations();
}
#endif

void ADreamlandZone::UpdatePlantSpotLocations()
{
	// Convert relative locations to world space
	for (FPlantSpot& Spot : PlantSpots)
	{
		Spot.Location = GetActorTransform().TransformPosition(Spot.Location);
	}
}

void ADreamlandZone::DrawDebugOutline()
{
	if (!GetWorld())
		return;

	FVector Origin, BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	// Draw persistent box outline
	DrawDebugBox(
		GetWorld(),
		Origin,
		BoxExtent,
		ZoneOutlineColor.ToFColor(true),
		true, // Persistent
		-1.f,
		0,
		10.f // Thickness
	);

	// Draw plant spots
	for (const FPlantSpot& Spot : PlantSpots)
	{
		FColor SpotColor = Spot.bIsHiddenSpot ? FColor::Orange : FColor::Cyan;
		
		DrawDebugSphere(
			GetWorld(),
			Spot.Location,
			PlantSpotRadius,
			12,
			SpotColor,
			true, // Persistent
			-1.f,
			0,
			5.f
		);

		// Draw text label
		DrawDebugString(
			GetWorld(),
			Spot.Location + FVector(0.f, 0.f, 100.f),
			Spot.SpotName,
			nullptr,
			SpotColor,
			-1.f, // Duration
			true
		);
	}

	UE_LOG(LogTemp, Log, TEXT("Dreamland Zone '%s' initialized with %d plant spots"), 
		*ZoneName, PlantSpots.Num());
}

bool ADreamlandZone::IsLocationInZone(const FVector& Location) const
{
	if (!ZoneVolume)
		return false;

	return ZoneVolume->IsOverlappingComponent(ZoneVolume);
}

bool ADreamlandZone::IsLocationValidPlantSpot(const FVector& Location, float Tolerance) const
{
	for (const FPlantSpot& Spot : PlantSpots)
	{
		float Distance = FVector::Dist(Location, Spot.Location);
		if (Distance <= Tolerance)
		{
			return true;
		}
	}

	return false;
}

FVector ADreamlandZone::GetNearestPlantSpot(const FVector& Location) const
{
	if (PlantSpots.Num() == 0)
		return GetActorLocation();

	FVector NearestSpot = PlantSpots[0].Location;
	float NearestDistance = FVector::Dist(Location, NearestSpot);

	for (const FPlantSpot& Spot : PlantSpots)
	{
		float Distance = FVector::Dist(Location, Spot.Location);
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestSpot = Spot.Location;
		}
	}

	return NearestSpot;
}

TArray<FVector> ADreamlandZone::GetAllPlantSpotLocations() const
{
	TArray<FVector> Locations;
	for (const FPlantSpot& Spot : PlantSpots)
	{
		Locations.Add(Spot.Location);
	}
	return Locations;
}

void ADreamlandZone::AddPlantSpotAtLocation(const FVector& WorldLocation, bool bIsHidden)
{
	FPlantSpot NewSpot;
	NewSpot.Location = WorldLocation;
	NewSpot.bIsHiddenSpot = bIsHidden;
	NewSpot.SpotName = FString::Printf(TEXT("Spot %d"), PlantSpots.Num() + 1);

	PlantSpots.Add(NewSpot);

	UE_LOG(LogTemp, Log, TEXT("Added plant spot '%s' at location"), *NewSpot.SpotName);

	if (bShowDebugOutline && GetWorld())
	{
		DrawDebugOutline();
	}
}
