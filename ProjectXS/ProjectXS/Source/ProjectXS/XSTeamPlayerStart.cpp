// Copyright Epic Games, Inc. All Rights Reserved.

#include "XSTeamPlayerStart.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

AXSTeamPlayerStart::AXSTeamPlayerStart()
{
	UpdateSpawnPointColor();
}

void AXSTeamPlayerStart::BeginPlay()
{
	Super::BeginPlay();
	UpdateSpawnPointColor();
}

#if WITH_EDITOR
void AXSTeamPlayerStart::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AXSTeamPlayerStart, Team))
	{
		UpdateSpawnPointColor();
	}
}
#endif

void AXSTeamPlayerStart::UpdateSpawnPointColor()
{
	// Update visual color based on team
	UArrowComponent* ArrowComponent = FindComponentByClass<UArrowComponent>();
	if (ArrowComponent)
	{
		if (Team == ETeam::Guardians)
		{
			// Blue for Guardians
			ArrowComponent->SetArrowColor(FLinearColor(0.2f, 0.5f, 1.0f));
		}
		else if (Team == ETeam::Devils)
		{
			// Red for Devils
			ArrowComponent->SetArrowColor(FLinearColor(1.0f, 0.2f, 0.2f));
		}
		else
		{
			// Gray for none
			ArrowComponent->SetArrowColor(FLinearColor(0.5f, 0.5f, 0.5f));
		}
	}

	UCapsuleComponent* CapsuleComponent = FindComponentByClass<UCapsuleComponent>();
	if (CapsuleComponent)
	{
		if (Team == ETeam::Guardians)
		{
			CapsuleComponent->ShapeColor = FColor(51, 128, 255); // Blue
		}
		else if (Team == ETeam::Devils)
		{
			CapsuleComponent->ShapeColor = FColor(255, 51, 51); // Red
		}
		else
		{
			CapsuleComponent->ShapeColor = FColor(128, 128, 128); // Gray
		}
	}
}
