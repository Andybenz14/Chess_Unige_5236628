// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_GameInstance.h"
#include "BasePiece.h"
#include "CHS_HumanPlayer.h"
#include <iostream>
#include "EngineUtils.h"
#include <string>
#include "Kismet/GameplayStatics.h"
#include <sstream>

FString UCHS_GameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}

void UCHS_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

FString UCHS_GameInstance::GetWinMessage()
{
	return WinMessage;
}

void UCHS_GameInstance::SetWinMessage(FString Message)
{
	WinMessage = Message;
}

FString UCHS_GameInstance::GetRegisterMove()
{
	return RegisterMove;
}

void UCHS_GameInstance::SetRegisterMove(FString Message)
{
	NewText = true;
	RegisterMove = Message;
	NewText = false;
	Moves.Add(Message);
}

bool UCHS_GameInstance::GetNewText()
{
	return NewText;
}

TArray<FString> UCHS_GameInstance::GetMovesArray() const
{

	return Moves;
}

void UCHS_GameInstance::SetUseMinimax(bool Choose)
{
	UseMinimax = Choose;
}

void UCHS_GameInstance::MoveInterpreterForReplay(FString SelectedMove)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->DisableInput(PlayerController);

	ACHS_HumanPlayer* HumanPlayer = Cast<ACHS_HumanPlayer>(*TActorIterator<ACHS_HumanPlayer>(GetWorld()));
	HumanPlayer->SetOriginalTileMaterial();

	std::string InputMove = TCHAR_TO_UTF8(*SelectedMove);
	std::istringstream Token(InputMove);
	std::string Color;
	
	Token >> Color >> Number;

	if (Color == "(Black)")
	{
		Number = 2 * Number;

	}
	else if (Color == "(White)")
	{
		Number = 2 * Number - 1;

	}

	for (const auto& Entry : PiecesStartingPosition)
	{
			
		FVector2D Position = Entry.Key;
		ABasePiece* Piece = Entry.Value;	

		if (Piece->IsHidden() == true)
		{
			Piece->SetActorHiddenInGame(false);

		}
		Piece->SetActorLocation(FVector(Position.X * 120, Position.Y * 120, 10));
	
				
	}

	for (int32 z = 0; z < PieceAfterPromo.Num(); z++)
	{
		PieceAfterPromo[z]->SetActorHiddenInGame(true);
	}

	for (int32 i = 0; i <= Number - 1; i++)
	{
		Replay(i);
	}	

}


void UCHS_GameInstance::ReturnToGameAfterReplay()
{
	for (int32 i = Number; i <= MovesForReplay.Num() - 1; i++)
	{
		Replay(i);
	}

	if (IsGameFinished == false)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		PlayerController->EnableInput(PlayerController);
	}
}

void UCHS_GameInstance::Replay(int32 i)
{
	FVector ActorLocation = MovesForReplay[i];
	FVector2D Normalized;
	Normalized.X = ActorLocation.X / 120;
	Normalized.Y = ActorLocation.Y / 120;

	ABasePiece* Actor = PiecesForReplay[i];
	Actor->SetActorLocation(ActorLocation);

	if (Moves[i].Contains("x"))
	{
		for (int j = 0; j < DestroyedPieceArray.Num(); j++)
		{
			if (DestroyedPieceArray[j].TurnCounter == i)
			{
				DestroyedPieceArray[j].Piece->SetActorHiddenInGame(true);
			}
		}

	}

	for (int j = 0; j < PromotedPieceArray.Num(); j++)
	{
		if (PromotedPieceArray[j].TurnCounter == i)
		{
			PromotedPieceArray[j].Piece->SetActorHiddenInGame(true);
			FVector Location = PromotedPieceArray[j].Piece->GetActorLocation();
			PieceAfterPromo[j]->SetActorLocation(Location);
			PieceAfterPromo[j]->SetActorHiddenInGame(false);
		}
	}
}


