// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "XSRoundBasedGameMode.h"
#include "XSTeamPlayerStart.generated.h"

/**
 * Team-specific spawn point for Guardians or Devils
 * Place these in your map to define where each team spawns
 */
UCLASS()
class PROJECTXS_API AXSTeamPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	AXSTeamPlayerStart();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	ETeam Team = ETeam::Guardians;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	int32 SpawnPriority = 0; // Higher = preferred

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void BeginPlay() override;
	
private:
	void UpdateSpawnPointColor();
};
