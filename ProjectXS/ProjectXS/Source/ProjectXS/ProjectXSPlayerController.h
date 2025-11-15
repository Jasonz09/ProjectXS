// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectXSPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UInputAction;
class UAbilitySystemComponent;

/**
 *  Player Controller with GAS integration for ability-based FPS
 *  Manages input mapping context and ability input binding.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract, config="Game")
class PROJECTXS_API AProjectXSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Constructor */
	AProjectXSPlayerController();

	// ====== Ability Input Actions ======
	
	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Abilities")
	UInputAction* FireAction;

	/** Alt fire input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Abilities")
	UInputAction* AltFireAction;

	/** Reload input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Abilities")
	UInputAction* ReloadAction;

	/** Ability 1 input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Abilities")
	UInputAction* Ability1Action;

	/** Ability 2 input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Abilities")
	UInputAction* Ability2Action;

	/** Ultimate ability input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Abilities")
	UInputAction* UltimateAction;

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** If true, the player will use UMG touch controls even if not playing on mobile platforms */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	/** Returns true if the player should use UMG touch controls */
	bool ShouldUseTouchControls() const;

protected:
	/** Bind ability inputs to AbilitySystemComponent */
	void BindAbilityInput();

	/** Ability input handlers */
	void OnFirePressed();
	void OnFireReleased();
	void OnAltFirePressed();
	void OnAltFireReleased();
	void OnReloadPressed();
	void OnAbility1Pressed();
	void OnAbility1Released();
	void OnAbility2Pressed();
	void OnAbility2Released();
	void OnUltimatePressed();
	void OnUltimateReleased();

	/** Get the ability system component from controlled pawn */
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
};
