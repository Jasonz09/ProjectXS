// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSRoundBasedGameMode.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AXSRoundBasedGameMode::AXSRoundBasedGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AXSRoundBasedGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Set rounds to win based on game mode
	if (GameModeType == EGameModeType::Competitive)
	{
		RoundsToWin = 7;
	}
	else // Unrated
	{
		RoundsToWin = 6;
	}

	// Start first round after a delay
	FTimerHandle StartDelayHandle;
	GetWorld()->GetTimerManager().SetTimer(StartDelayHandle, this, &AXSRoundBasedGameMode::StartNewRound, 3.f, false);
}

void AXSRoundBasedGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update phase timer
	if (PhaseTimeRemaining > 0.f)
	{
		PhaseTimeRemaining -= DeltaTime;
		
		if (PhaseTimeRemaining <= 0.f)
		{
			OnPhaseTimerExpired();
		}
	}
}

void AXSRoundBasedGameMode::StartNewRound()
{
	CurrentRound++;
	ArtilsPlanted = 0;
	
	// Reset player counts
	GuardiansAlive = 0;
	DevilsAlive = 0;
	
	// Count alive players
	UpdateAliveCounts();
	
	// Start buy phase
	ChangePhase(ERoundPhase::BuyPhase);
	
	UE_LOG(LogTemp, Log, TEXT("Round %d started - Guardians: %d vs Devils: %d"), 
		CurrentRound, GuardiansScore, DevilsScore);
}

void AXSRoundBasedGameMode::ChangePhase(ERoundPhase NewPhase)
{
	CurrentPhase = NewPhase;
	OnRoundPhaseChanged.Broadcast(NewPhase);
	
	switch (NewPhase)
	{
		case ERoundPhase::BuyPhase:
			PhaseTimeRemaining = BuyPhaseDuration;
			UE_LOG(LogTemp, Log, TEXT("Buy Phase Started - %f seconds"), BuyPhaseDuration);
			break;
			
		case ERoundPhase::CombatPhase:
			PhaseTimeRemaining = CombatPhaseDuration;
			UE_LOG(LogTemp, Log, TEXT("Combat Phase Started - %f seconds"), CombatPhaseDuration);
			break;
			
		case ERoundPhase::PostPlantPhase:
			PhaseTimeRemaining = PostPlantDuration;
			UE_LOG(LogTemp, Log, TEXT("Post-Plant Phase Started - %f seconds to defuse!"), PostPlantDuration);
			break;
			
		case ERoundPhase::RoundEnd:
			PhaseTimeRemaining = 0.f;
			break;
	}
}

void AXSRoundBasedGameMode::OnPhaseTimerExpired()
{
	switch (CurrentPhase)
	{
		case ERoundPhase::BuyPhase:
			// Buy phase ended, start combat
			ChangePhase(ERoundPhase::CombatPhase);
			break;
			
		case ERoundPhase::CombatPhase:
			// Combat phase ended with no plants - Guardians win
			EndRound(ETeam::Guardians);
			break;
			
		case ERoundPhase::PostPlantPhase:
			// Timer expired, Artils detonated - Devils win
			OnBothArtilsDetonated();
			break;
			
		default:
			break;
	}
}

void AXSRoundBasedGameMode::OnPlayerKilled(AController* Victim, AController* Killer)
{
	if (!Victim) return;
	
	ETeam VictimTeam = GetPlayerTeam(Victim);
	
	// Update alive counts
	if (VictimTeam == ETeam::Guardians)
	{
		GuardiansAlive = FMath::Max(0, GuardiansAlive - 1);
	}
	else if (VictimTeam == ETeam::Devils)
	{
		DevilsAlive = FMath::Max(0, DevilsAlive - 1);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Player killed - Guardians: %d, Devils: %d"), GuardiansAlive, DevilsAlive);
	
	// Check if one team is eliminated
	CheckRoundEndConditions();
}

void AXSRoundBasedGameMode::OnArtilPlanted()
{
	ArtilsPlanted++;
	
	UE_LOG(LogTemp, Warning, TEXT("Artil planted! (%d/2)"), ArtilsPlanted);
	
	// If both Artils are planted, start post-plant phase
	if (ArtilsPlanted >= 2)
	{
		ChangePhase(ERoundPhase::PostPlantPhase);
		UE_LOG(LogTemp, Warning, TEXT("Both Artils planted! Guardians have %f seconds to defuse!"), PostPlantDuration);
	}
}

void AXSRoundBasedGameMode::OnArtilDefused()
{
	ArtilsPlanted = FMath::Max(0, ArtilsPlanted - 1);
	
	UE_LOG(LogTemp, Log, TEXT("Artil defused! Remaining: %d"), ArtilsPlanted);
	
	// If at least one Artil is defused, Guardians win
	EndRound(ETeam::Guardians);
}

void AXSRoundBasedGameMode::OnBothArtilsDetonated()
{
	UE_LOG(LogTemp, Warning, TEXT("Both Artils detonated! Devils win the round!"));
	EndRound(ETeam::Devils);
}

void AXSRoundBasedGameMode::CheckRoundEndConditions()
{
	// Don't check during buy phase or round end
	if (CurrentPhase == ERoundPhase::BuyPhase || CurrentPhase == ERoundPhase::RoundEnd)
		return;
	
	// Check if all Guardians are eliminated
	if (GuardiansAlive <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Guardians eliminated! Devils win!"));
		EndRound(ETeam::Devils);
		return;
	}
	
	// Check if all Devils are eliminated
	if (DevilsAlive <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Devils eliminated! Guardians win!"));
		EndRound(ETeam::Guardians);
		return;
	}
}

void AXSRoundBasedGameMode::EndRound(ETeam WinningTeam)
{
	ChangePhase(ERoundPhase::RoundEnd);
	
	// Award point to winning team
	if (WinningTeam == ETeam::Guardians)
	{
		GuardiansScore++;
		OnTeamScored.Broadcast(ETeam::Guardians, GuardiansScore);
		UE_LOG(LogTemp, Display, TEXT("Guardians win round %d! Score: Guardians %d - Devils %d"), 
			CurrentRound, GuardiansScore, DevilsScore);
	}
	else if (WinningTeam == ETeam::Devils)
	{
		DevilsScore++;
		OnTeamScored.Broadcast(ETeam::Devils, DevilsScore);
		UE_LOG(LogTemp, Display, TEXT("Devils win round %d! Score: Guardians %d - Devils %d"), 
			CurrentRound, GuardiansScore, DevilsScore);
	}
	
	// Check if match is over
	if (GuardiansScore >= RoundsToWin)
	{
		OnMatchEnded.Broadcast(ETeam::Guardians);
		UE_LOG(LogTemp, Warning, TEXT("MATCH OVER! Guardians win %d-%d!"), GuardiansScore, DevilsScore);
		return;
	}
	else if (DevilsScore >= RoundsToWin)
	{
		OnMatchEnded.Broadcast(ETeam::Devils);
		UE_LOG(LogTemp, Warning, TEXT("MATCH OVER! Devils win %d-%d!"), DevilsScore, GuardiansScore);
		return;
	}
	
	// Start next round after delay
	FTimerHandle NextRoundHandle;
	GetWorld()->GetTimerManager().SetTimer(NextRoundHandle, this, &AXSRoundBasedGameMode::StartNewRound, 5.f, false);
}

void AXSRoundBasedGameMode::UpdateAliveCounts()
{
	GuardiansAlive = 0;
	DevilsAlive = 0;
	
	// Count all players
	for (const auto& PlayerTeamPair : PlayerTeams)
	{
		if (PlayerTeamPair.Key && !PlayerTeamPair.Key->GetPawn())
			continue;
			
		if (PlayerTeamPair.Value == ETeam::Guardians)
			GuardiansAlive++;
		else if (PlayerTeamPair.Value == ETeam::Devils)
			DevilsAlive++;
	}
}

ETeam AXSRoundBasedGameMode::GetPlayerTeam(AController* PlayerController) const
{
	if (!PlayerController) return ETeam::None;
	
	const ETeam* Team = PlayerTeams.Find(PlayerController);
	return Team ? *Team : ETeam::None;
}

void AXSRoundBasedGameMode::AssignPlayerToTeam(AController* PlayerController, ETeam Team)
{
	if (!PlayerController) return;
	
	PlayerTeams.Add(PlayerController, Team);
	UE_LOG(LogTemp, Log, TEXT("Player assigned to team: %s"), 
		Team == ETeam::Guardians ? TEXT("Guardians") : TEXT("Devils"));
	
	UpdateAliveCounts();
}
