// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "XSRoundBasedGameMode.generated.h"

class AArtilActor;
class ADreamlandZone;

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	Unrated UMETA(DisplayName = "Unrated"),
	Competitive UMETA(DisplayName = "Competitive")
};

UENUM(BlueprintType)
enum class ERoundPhase : uint8
{
	BuyPhase UMETA(DisplayName = "Buy Phase"),
	CombatPhase UMETA(DisplayName = "Combat Phase"),
	PostPlantPhase UMETA(DisplayName = "Post-Plant Phase"),
	RoundEnd UMETA(DisplayName = "Round End")
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
	None UMETA(DisplayName = "None"),
	Guardians UMETA(DisplayName = "Guardians"),
	Devils UMETA(DisplayName = "Devils")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundPhaseChanged, ERoundPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamScored, ETeam, ScoringTeam, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchEnded, ETeam, WinningTeam);

/**
 * Round-based game mode for Unrated and Competitive matches
 * - 6v6 (Guardians vs Devils)
 * - First to 6 rounds (Unrated) or 7 rounds (Competitive)
 * - Win by eliminating all enemies OR planting both Artils OR defusing
 */
UCLASS()
class PROJECTXS_API AXSRoundBasedGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AXSRoundBasedGameMode();

	// ====== GAME MODE SETTINGS ======
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode")
	EGameModeType GameModeType = EGameModeType::Unrated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode")
	int32 RoundsToWin = 6; // 6 for Unrated, 7 for Competitive

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode")
	int32 PlayersPerTeam = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode")
	float BuyPhaseDuration = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode")
	float CombatPhaseDuration = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode")
	float PostPlantDuration = 50.f;

	// ====== CURRENT GAME STATE ======
	
	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	ERoundPhase CurrentPhase = ERoundPhase::BuyPhase;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 GuardiansScore = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 DevilsScore = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 CurrentRound = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 GuardiansAlive = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 DevilsAlive = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Game State")
	int32 ArtilsPlanted = 0;

	// ====== EVENTS ======
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRoundPhaseChanged OnRoundPhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTeamScored OnTeamScored;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMatchEnded OnMatchEnded;

	// ====== PUBLIC FUNCTIONS ======
	
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void StartNewRound();

	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void OnPlayerKilled(AController* Victim, AController* Killer);

	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void OnArtilPlanted();

	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void OnArtilDefused();

	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void OnBothArtilsDetonated();

	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	ETeam GetPlayerTeam(AController* PlayerController) const;

	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void AssignPlayerToTeam(AController* PlayerController, ETeam Team);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// Round management
	void ChangePhase(ERoundPhase NewPhase);
	void EndRound(ETeam WinningTeam);
	void CheckRoundEndConditions();
	void UpdateAliveCounts();
	
	// Team assignment
	TMap<AController*, ETeam> PlayerTeams;
	
	// Timers
	float PhaseTimeRemaining = 0.f;
	FTimerHandle PhaseTimerHandle;
	
	void OnPhaseTimerExpired();
};
