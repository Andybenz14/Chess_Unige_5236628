// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_PlayerController.h"

#include "Components/InputComponent.h"

ACHS_PlayerController::ACHS_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ACHS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(TTTContext, 0);
	}
}

void ACHS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ACHS_PlayerController::ClickOnGrid);
	}
}

void ACHS_PlayerController::ClickOnGrid()
{
	const auto HumanPlayer = Cast<ACHS_HumanPlayer>(GetPawn());
	if (IsValid(HumanPlayer))
	{
		HumanPlayer->OnClick();
	}
}