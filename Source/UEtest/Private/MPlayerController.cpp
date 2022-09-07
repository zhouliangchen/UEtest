// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "MCharacter.h"
#include "Blueprint/UserWidget.h"


void AMPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AMCharacter* InPlayer = Cast<AMCharacter>(InPawn))
	{
		InPlayer->OnServerPlayerSpawned();
	}
}

void AMPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateReplicatedBP(PlayerState);
}

void AMPlayerController::TogglePauseMenu()
{
	if (PauseMenuWidgetInstance && PauseMenuWidgetInstance->IsInViewport())
	{
		PauseMenuWidgetInstance->RemoveFromParent();
		PauseMenuWidgetInstance = nullptr;
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		if (!PauseMenuWidgetInstance && PauseMenuWidgetClass)
		{
			PauseMenuWidgetInstance = CreateWidget(this, PauseMenuWidgetClass);
		}
		if (PauseMenuWidgetInstance)
		{
			PauseMenuWidgetInstance->AddToViewport(100);
			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Pause", IE_Pressed, this, &AMPlayerController::TogglePauseMenu);
}
