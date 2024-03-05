// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CHS_GameInstance.generated.h"



USTRUCT() struct FDestroyedPiece
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		ABasePiece* Piece;

	UPROPERTY(Transient)
		FVector2D Position;
};

USTRUCT() struct FPromotedPiece
{
	GENERATED_BODY()

		UPROPERTY(Transient)
		ABasePiece* Piece;

	UPROPERTY(Transient)
		FVector2D Position;
};





UCLASS()
class CHESS_UNIGE_5236628_API UCHS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString CurrentTurnMessage = "Current Player";

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetTurnMessage();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetTurnMessage(FString Message);

	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString WinMessage = "";

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetWinMessage();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetWinMessage(FString Message);

	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString RegisterMove = "";

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetRegisterMove();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetRegisterMove(FString Message);
	
	UFUNCTION(BlueprintCallable)
		bool GetNewText();

	UPROPERTY(EditAnywhere)
		bool NewText = false;

	UFUNCTION(BlueprintCallable)
		TArray<FString> GetMovesArray() const;

	UPROPERTY(EditDefaultsOnly)
		TArray<FString> Moves;

	UFUNCTION(BlueprintCallable)
		void MoveInterpreterForReplay(FString SelectedMove);

	UFUNCTION(BlueprintCallable)
		void ReturnToGameAfterReplay();

	UPROPERTY(Transient)
		TArray<FVector> MovesForReplay;

	UPROPERTY(Transient)
		TArray<ABasePiece*> PiecesForReplay;
	
	UPROPERTY(Transient)
		TMap<FVector2D, ABasePiece*> PiecesStartingPosition;

	UPROPERTY(Transient)
		TArray<FDestroyedPiece> DestroyedPieceArray;

	UPROPERTY(Transient)
		TArray<FPromotedPiece> PromotedPieceArray;

	UPROPERTY(Transient)
		TArray<ABasePiece*> PieceAfterPromo;

	UPROPERTY(Transient)
		int32 DestroyedPieceArrayIndexCounter = 0;

	int Number = 1;

};
