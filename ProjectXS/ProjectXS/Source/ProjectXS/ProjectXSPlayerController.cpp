// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectXSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "ProjectXSCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "ProjectXS.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "AbilitySystemComponent.h"
#include "XSAbilityCharacter.h"

AProjectXSPlayerController::AProjectXSPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AProjectXSCameraManager::StaticClass();
}

void AProjectXSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogProjectXS, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AProjectXSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}

		// Bind ability inputs
		BindAbilityInput();
	}
	
}

bool AProjectXSPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AProjectXSPlayerController::BindAbilityInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind ability inputs
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AProjectXSPlayerController::OnFirePressed);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AProjectXSPlayerController::OnFireReleased);
		}

		if (AltFireAction)
		{
			EnhancedInputComponent->BindAction(AltFireAction, ETriggerEvent::Started, this, &AProjectXSPlayerController::OnAltFirePressed);
			EnhancedInputComponent->BindAction(AltFireAction, ETriggerEvent::Completed, this, &AProjectXSPlayerController::OnAltFireReleased);
		}

		if (ReloadAction)
		{
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AProjectXSPlayerController::OnReloadPressed);
		}

		if (Ability1Action)
		{
			EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Started, this, &AProjectXSPlayerController::OnAbility1Pressed);
			EnhancedInputComponent->BindAction(Ability1Action, ETriggerEvent::Completed, this, &AProjectXSPlayerController::OnAbility1Released);
		}

		if (Ability2Action)
		{
			EnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Started, this, &AProjectXSPlayerController::OnAbility2Pressed);
			EnhancedInputComponent->BindAction(Ability2Action, ETriggerEvent::Completed, this, &AProjectXSPlayerController::OnAbility2Released);
		}

		if (UltimateAction)
		{
			EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Started, this, &AProjectXSPlayerController::OnUltimatePressed);
			EnhancedInputComponent->BindAction(UltimateAction, ETriggerEvent::Completed, this, &AProjectXSPlayerController::OnUltimateReleased);
		}
	}
}

UAbilitySystemComponent* AProjectXSPlayerController::GetAbilitySystemComponent() const
{
	if (AXSAbilityCharacter* Character = Cast<AXSAbilityCharacter>(GetPawn()))
	{
		return Character->GetAbilitySystemComponent();
	}
	return nullptr;
}

void AProjectXSPlayerController::OnFirePressed()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputPressed(3); // Input ID 3 for primary fire
	}
}

void AProjectXSPlayerController::OnFireReleased()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputReleased(3);
	}
}

void AProjectXSPlayerController::OnAltFirePressed()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputPressed(4); // Input ID 4 for alt fire
	}
}

void AProjectXSPlayerController::OnAltFireReleased()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputReleased(4);
	}
}

void AProjectXSPlayerController::OnReloadPressed()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputPressed(5); // Input ID 5 for reload
	}
}

void AProjectXSPlayerController::OnAbility1Pressed()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputPressed(0); // Input ID 0 for primary ability
	}
}

void AProjectXSPlayerController::OnAbility1Released()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputReleased(0);
	}
}

void AProjectXSPlayerController::OnAbility2Pressed()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputPressed(1); // Input ID 1 for secondary ability
	}
}

void AProjectXSPlayerController::OnAbility2Released()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputReleased(1);
	}
}

void AProjectXSPlayerController::OnUltimatePressed()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputPressed(2); // Input ID 2 for ultimate
	}
}

void AProjectXSPlayerController::OnUltimateReleased()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityLocalInputReleased(2);
	}
}
