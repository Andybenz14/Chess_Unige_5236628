// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_GameMode.h"
#include "CHS_HumanPlayer.h"
#include "CHS_PlayerController.h"
#include "CHS_RandomPlayer.h"
#include "MinimaxPlayer.h"
#include "EngineUtils.h"

ACHS_GameMode::ACHS_GameMode()
{
	PlayerControllerClass = ACHS_PlayerController::StaticClass();
	// Human player is spawned by default
	DefaultPawnClass = ACHS_HumanPlayer::StaticClass();
	FieldSize = 8;
	GameInstance = Cast<UCHS_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}


void ACHS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	IsMyTurn = 1;

	// Find human player pointer
	ACHS_HumanPlayer* HumanPlayer = Cast<ACHS_HumanPlayer>(*TActorIterator<ACHS_HumanPlayer>(GetWorld()));


	if (GameFieldClass != nullptr)
	{
		// Spawn chessboard
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		GField->Size = FieldSize;

	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	// Player Camera set
	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);

	// Set human player location
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// If player selected to play against minimax in the main menu
	if (GameInstance->UseMinimax)
	{
		// Spawn Minimax and get pointer to it
		AMinimaxPlayer* AI = GetWorld()->SpawnActor<AMinimaxPlayer>(FVector(), FRotator());
		
	}
	else
	{
		// Spawn Random and get pointer to it
		ACHS_RandomPlayer* AI = GetWorld()->SpawnActor<ACHS_RandomPlayer>(FVector(), FRotator());
	
	}
	
	// Human always starts
	if (IsMyTurn)
	{
		// Calls human player turn function
		HumanPlayer->OnTurn();
	}
	

	
}

// Called after human move
void ACHS_GameMode::EndHumanTurn()
{
	IsMyTurn = 0;

	// Disable input 
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0); 
	PlayerController->DisableInput(PlayerController);
	
	// Check if game can continue
	if (GameInstance->IsGameFinished == false)
	{
		// If the enemy is minimax
		if (GameInstance->UseMinimax)
		{
			// Cast to minimax
			AMinimaxPlayer* AI = Cast<AMinimaxPlayer>(*TActorIterator<AMinimaxPlayer>(GetWorld()));

			// Minimax on turn function
			AI->OnTurn();
		}
		else
		{
			// Cast to random
			ACHS_RandomPlayer* AI = Cast<ACHS_RandomPlayer>(*TActorIterator<ACHS_RandomPlayer>(GetWorld()));

			// Random on turn function
			AI->OnTurn();
		}

	}
}

// Called after AI move
void ACHS_GameMode::EndAITurn()
{
	IsMyTurn = 1;

	// Cast to human player and enable input
	ACHS_HumanPlayer* HumanPlayer = Cast<ACHS_HumanPlayer>(*TActorIterator<ACHS_HumanPlayer>(GetWorld()));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->EnableInput(PlayerController);
	
	// Check if game continue 
	if (GameInstance->IsGameFinished == false)
	{
		// Human on turn function
		HumanPlayer->OnTurn();
	}
}


