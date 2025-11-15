// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "XSCharacterData.generated.h"

class AXSAbilityCharacter;
class UTexture2D;

/**
 * Character data asset for lobby selection
 * Contains character info, portrait, and stats for display
 */
UCLASS(BlueprintType)
class PROJECTXS_API UXSCharacterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Character class to spawn in match */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	TSubclassOf<AXSAbilityCharacter> CharacterClass;

	/** Display name */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	FText CharacterName;

	/** Role/Class (Duelist, Demolitionist, etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	FText CharacterRole;

	/** Character description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	FText CharacterDescription;

	/** Portrait image for character selection */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	UTexture2D* CharacterPortrait;

	/** Full body image for details view */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	UTexture2D* CharacterFullImage;

	/** Character accent color (for UI theming) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	FLinearColor AccentColor;

	// Stats for display

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	int32 BaseHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	int32 BaseEnergy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MobilityRating; // 0-10

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float DamageRating; // 0-10

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float RangeRating; // 0-10

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float DifficultyRating; // 0-10

	// Ability info for display

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FText PassiveAbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FText PassiveAbilityDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FText PrimaryAbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FText PrimaryAbilityDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FText SecondaryAbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FText SecondaryAbilityDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FText UltimateAbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FText UltimateAbilityDescription;
};
