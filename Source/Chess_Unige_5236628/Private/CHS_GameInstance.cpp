// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_GameInstance.h"
#include "BasePiece.h"
#include "CHS_HumanPlayer.h"
#include <iostream>
#include "EngineUtils.h"
#include <string>
#include "Kismet/GameplayStatics.h"
#include <sstream>

// Called in BP_User_Widget  
FString UCHS_GameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}
// set turn message for widget
void UCHS_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

// Called in BP_User_Widget  
FString UCHS_GameInstance::GetWinMessage()
{
	return WinMessage;
}

//set win message for widget
void UCHS_GameInstance::SetWinMessage(FString Message)
{
	WinMessage = Message;
}

// get register last move 
FString UCHS_GameInstance::GetRegisterMove()
{
	return RegisterMove;
}

// set new register move add to moves array
void UCHS_GameInstance::SetRegisterMove(FString Message)
{
	NewText = true;
	RegisterMove = Message;
	NewText = false;
	Moves.Add(Message);
}

// get bool
bool UCHS_GameInstance::GetNewText()
{
	return NewText;
}

// Called in BP_User_Widget to fill the register moves box 
TArray<FString> UCHS_GameInstance::GetMovesArray() const
{

	return Moves;
}

// Called in Main menu widget
void UCHS_GameInstance::SetUseMinimax(bool Choose)
{
	UseMinimax = Choose;
}

// Called in BP_User_Widget when user selects the move who wants to replay
void UCHS_GameInstance::MoveInterpreterForReplay(FString SelectedMove)
{
	// Disable player input during Replay
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->DisableInput(PlayerController);

	// Reset Tiles material for safety. (User could have clicked on replay while possible moves material where applied)
	ACHS_HumanPlayer* HumanPlayer = Cast<ACHS_HumanPlayer>(*TActorIterator<ACHS_HumanPlayer>(GetWorld()));
	HumanPlayer->SetOriginalTileMaterial();

	// Small tokenizer for the move selected by the user 
	std::string InputMove = TCHAR_TO_UTF8(*SelectedMove);
	std::istringstream Token(InputMove);
	// Takes the player' color who did the move
	std::string Color;
	
	// Need the turn index
	Token >> Color >> Number;

	// If string starts with ...
	if (Color == "(Black)")
	{
		// Because black is aways the second player to play 2*turn = game moves index. ( 1.)Black -> 1*2 = 2nd move)
		Number = 2 * Number;

	}
	else if (Color == "(White)")
	{
		// Because white is aways the first player to play 2*turn - 1 = game moves index. ( 2.)White -> 2*2 - 1 = 3rd move)
		Number = 2 * Number - 1;

	}

	// Move pieces into starting position
	for (const auto& Entry : PiecesStartingPosition)
	{
			
		FVector2D Position = Entry.Key;
		ABasePiece* Piece = Entry.Value;	

		// Reset pieces visible
		if (Piece->IsHidden() == true)
		{
			Piece->SetActorHiddenInGame(false);

		}
		// Reset starting position
		Piece->SetActorLocation(FVector(Position.X * 120, Position.Y * 120, 10));
	
				
	}
	// Set pieces after promotion as hidden
	for (int32 z = 0; z < PieceAfterPromo.Num(); z++)
	{
		PieceAfterPromo[z]->SetActorHiddenInGame(true);
	}
	// For each move call replay until the move selected by the user
	for (int32 i = 0; i <= Number - 1; i++)
	{
		Replay(i);
	}	

}

// Called when user clicks on Return to play button 
void UCHS_GameInstance::ReturnToGameAfterReplay()
{
	// Calls replay for each move until last move
	for (int32 i = Number; i <= MovesForReplay.Num() - 1; i++)
	{
		Replay(i);
	}

	// Enable user input to continue playing
	if (IsGameFinished == false)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		PlayerController->EnableInput(PlayerController);
	}
}

// Re-execute moves moving pieces
void UCHS_GameInstance::Replay(int32 i)
{
	// Select the move position
	FVector ActorLocation = MovesForReplay[i];
	FVector2D Normalized;
	Normalized.X = ActorLocation.X / 120;
	Normalized.Y = ActorLocation.Y / 120;

	// Select the actor to move
	ABasePiece* Actor = PiecesForReplay[i];
	// Move the actor 
	Actor->SetActorLocation(ActorLocation);

	// If the selected move is a kill move
	if (Moves[i].Contains("x"))
	{
		// Iterate until it finds the destroyed piece at this turn
		for (int j = 0; j < DestroyedPieceArray.Num(); j++)
		{
			if (DestroyedPieceArray[j].TurnCounter == i)
			{
				// Set the killed actor at this turn as hidden
				DestroyedPieceArray[j].Piece->SetActorHiddenInGame(true);
			}
		}

	}
	// Iterate until it finds (if not there is no promotion) a Promoted piece at this turn
	for (int j = 0; j < PromotedPieceArray.Num(); j++)
	{
		if (PromotedPieceArray[j].TurnCounter == i)
		{
			// Set pawn as hidden
			PromotedPieceArray[j].Piece->SetActorHiddenInGame(true);
			FVector Location = PromotedPieceArray[j].Piece->GetActorLocation();

			// Find the promoted piece and sets his location as the same of the pawn
			PieceAfterPromo[j]->SetActorLocation(Location);

			// Sets piece visible
			PieceAfterPromo[j]->SetActorHiddenInGame(false);
		}
	}
}


