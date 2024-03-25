// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CHS_GameInstance.generated.h"


// Struct to track pieces after destruction. Useful for replay
USTRUCT() struct FDestroyedPiece
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		ABasePiece* Piece;

	UPROPERTY(Transient)
		FVector2D Position;

	UPROPERTY(Transient)
		int32 TurnCounter;
};
// Struct to track pieces befor them promotion. Useful for replay
USTRUCT() struct FPromotedPiece
{
	GENERATED_BODY()

		UPROPERTY(Transient)
		ABasePiece* Piece;

	UPROPERTY(Transient)
		FVector2D Position;

	UPROPERTY(Transient)
		int32 TurnCounter;
};





UCLASS()
class CHESS_UNIGE_5236628_API UCHS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// Store all in-game moves
	UPROPERTY(Transient)
		TArray<FVector> MovesForReplay;

	// Store all pieces moved in-game
	UPROPERTY(Transient)
		TArray<ABasePiece*> PiecesForReplay;

	// Store at the begining the starting position
	UPROPERTY(Transient)
		TMap<FVector2D, ABasePiece*> PiecesStartingPosition;

	// Array of Struct
	UPROPERTY(Transient)
		TArray<FDestroyedPiece> DestroyedPieceArray;

	// Array of Struct
	UPROPERTY(Transient)
		TArray<FPromotedPiece> PromotedPieceArray;

	// Keeps track of pieces after promotion
	UPROPERTY(Transient)
		TArray<ABasePiece*> PieceAfterPromo;

	// Is a turn counter used to define .turncounter of the struct
	UPROPERTY(Transient)
		int32 DestroyedPieceArrayIndexCounter = 0;

	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString CurrentTurnMessage = "Current Player";

	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString WinMessage = "";

	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString RegisterMove = "";

	UPROPERTY(EditAnywhere)
		bool NewText = false;

	// Defined at main menu to selected against which enemy play.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool UseMinimax;

	// Defined at main menu to selected the game modality
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool UseTimeAttack;

	// Array of all match moves
	UPROPERTY(EditDefaultsOnly)
		TArray<FString> Moves;

	// Turn counter used by MoveInterpreterForReplay()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Number = 1;

	// Defined by IsCheckMate()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsGameFinished = false;

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetRegisterMove();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetRegisterMove(FString Message);
	
	UFUNCTION(BlueprintCallable)
		bool GetNewText();

	// Get moves array
	UFUNCTION(BlueprintCallable)
		TArray<FString> GetMovesArray() const;

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetTurnMessage();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetTurnMessage(FString Message);

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetWinMessage();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetWinMessage(FString Message);

	// Used by main menu widget to selected the enemy
	UFUNCTION(BlueprintCallable)
		void SetUseMinimax(bool Choose);

	// Used by main menu widget to selected the game modality
	UFUNCTION(BlueprintCallable)
		void SetUseTimeAttack(bool Choose);

	// Interpret the move string ( selected by user in game), reset the chessboard to starting position
	// and calls replay. It disable user input
	UFUNCTION(BlueprintCallable)
		void MoveInterpreterForReplay(FString SelectedMove);

	// From the selected replay state, reset the last move state and enable user to play again
	UFUNCTION(BlueprintCallable)
		void ReturnToGameAfterReplay();

	// Moves pieces into the turn (index) chessboard state
	UFUNCTION(BlueprintCallable)
		void Replay(int32 index);
	
};
