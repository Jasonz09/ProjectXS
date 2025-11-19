// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArtilActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AArtilActor::AArtilActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	// Create collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(50.f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = CollisionSphere;

	// Create mesh
	ArtilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArtilMesh"));
	ArtilMesh->SetupAttachment(RootComponent);
	ArtilMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AArtilActor::BeginPlay()
{
	Super::BeginPlay();
	ChangeState(EArtilState::Dropped);
}

void AArtilActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Only server handles logic
	if (!HasAuthority())
		return;

	switch (CurrentState)
	{
		case EArtilState::Planting:
			UpdatePlantingProgress(DeltaTime);
			break;

		case EArtilState::Planted:
			UpdateDetonationCountdown(DeltaTime);
			break;

		case EArtilState::Defusing:
			UpdateDefusingProgress(DeltaTime);
			break;

		default:
			break;
	}
}

void AArtilActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArtilActor, CurrentState);
	DOREPLIFETIME(AArtilActor, CurrentCarrier);
}

void AArtilActor::ChangeState(EArtilState NewState)
{
	if (CurrentState == NewState)
		return;

	CurrentState = NewState;
	OnArtilStateChanged.Broadcast(NewState);

	UE_LOG(LogTemp, Log, TEXT("Artil state changed to: %d"), (int32)NewState);
}

void AArtilActor::OnRep_ArtilState()
{
	OnArtilStateChanged.Broadcast(CurrentState);
}

bool AArtilActor::CanBePickedUp(AActor* PickupActor) const
{
	// Only Devils can pick up Artils
	// Only when dropped or if previous carrier died
	return CurrentState == EArtilState::Dropped && PickupActor != nullptr;
}

void AArtilActor::PickUp(AActor* NewCarrier)
{
	if (!HasAuthority() || !NewCarrier)
		return;

	CurrentCarrier = NewCarrier;
	ChangeState(EArtilState::Carried);

	// Attach to carrier
	AttachToActor(NewCarrier, FAttachmentTransformRules::SnapToTargetIncludingScale);
	SetActorRelativeLocation(FVector(0.f, 0.f, 50.f)); // Offset above carrier

	// Disable physics
	ArtilMesh->SetSimulatePhysics(false);

	UE_LOG(LogTemp, Warning, TEXT("Artil picked up by %s"), *NewCarrier->GetName());
}

void AArtilActor::Drop(const FVector& DropLocation)
{
	if (!HasAuthority())
		return;

	CurrentCarrier = nullptr;
	ChangeState(EArtilState::Dropped);

	// Detach from carrier
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorLocation(DropLocation);

	// Enable physics
	ArtilMesh->SetSimulatePhysics(true);

	UE_LOG(LogTemp, Warning, TEXT("Artil dropped at location"));
}

bool AArtilActor::CanBePlanted(const FVector& PlantLocation) const
{
	// TODO: Check if location is within a Dreamland zone
	// For now, just return true if carried
	return CurrentState == EArtilState::Carried;
}

void AArtilActor::StartPlanting(AActor* Planter)
{
	if (!HasAuthority() || CurrentState != EArtilState::Carried)
		return;

	CurrentPlanter = Planter;
	ChangeState(EArtilState::Planting);
	PlantProgress = 0.f;

	UE_LOG(LogTemp, Warning, TEXT("Started planting Artil..."));
}

void AArtilActor::InterruptPlanting()
{
	if (!HasAuthority())
		return;

	if (CurrentState == EArtilState::Planting)
	{
		ChangeState(EArtilState::Carried);
		PlantProgress = 0.f;
		CurrentPlanter = nullptr;

		UE_LOG(LogTemp, Warning, TEXT("Planting interrupted!"));
	}
}

void AArtilActor::UpdatePlantingProgress(float DeltaTime)
{
	PlantProgress += DeltaTime;

	if (PlantProgress >= PlantDuration)
	{
		// Planting complete!
		CurrentCarrier = nullptr;
		CurrentPlanter = nullptr;
		ChangeState(EArtilState::Planted);
		DetonationCountdown = DetonationTime;

		// Detach and place on ground
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		OnPlanted.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Artil planted! Detonation in %f seconds!"), DetonationTime);
	}
}

void AArtilActor::StartDefusing(AActor* Defuser)
{
	if (!HasAuthority() || CurrentState != EArtilState::Planted)
		return;

	CurrentDefuser = Defuser;
	ChangeState(EArtilState::Defusing);
	DefuseProgress = 0.f;

	UE_LOG(LogTemp, Warning, TEXT("Started defusing Artil..."));
}

void AArtilActor::InterruptDefusing()
{
	if (!HasAuthority())
		return;

	if (CurrentState == EArtilState::Defusing)
	{
		ChangeState(EArtilState::Planted);
		// Keep defuse progress (partial defusing allowed)
		CurrentDefuser = nullptr;

		UE_LOG(LogTemp, Warning, TEXT("Defusing interrupted! Progress saved: %f%%"), 
			(DefuseProgress / DefuseDuration) * 100.f);
	}
}

void AArtilActor::UpdateDefusingProgress(float DeltaTime)
{
	DefuseProgress += DeltaTime;

	if (DefuseProgress >= DefuseDuration)
	{
		// Defusing complete!
		CurrentDefuser = nullptr;
		ChangeState(EArtilState::Defused);

		OnDefused.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Artil defused!"));

		// Notify game mode
		// Game mode will handle scoring
	}
}

void AArtilActor::UpdateDetonationCountdown(float DeltaTime)
{
	DetonationCountdown -= DeltaTime;

	if (DetonationCountdown <= 0.f)
	{
		Detonate();
	}
}

void AArtilActor::Detonate()
{
	if (!HasAuthority())
		return;

	ChangeState(EArtilState::Detonated);
	OnDetonated.Broadcast();

	UE_LOG(LogTemp, Error, TEXT("ARTIL DETONATED!"));

	// Apply radial damage
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		ExplosionDamage,
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(), // Ignore list
		this,
		nullptr,
		true // Full damage
	);

	// TODO: Spawn explosion VFX and SFX

	// Destroy after a short delay to allow VFX to play
	SetLifeSpan(2.f);
}
