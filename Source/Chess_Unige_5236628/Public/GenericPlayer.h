// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameField.h"
#include "BasePiece.h"
#include "CHS_GameMode.h"
#include "CHS_GameInstance.h"
#include "CHS_PlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "GenericPlayer.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API AGenericPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGenericPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditDefaultsOnly)
	UCHS_GameInstance* GameInstance;

	// Possible moves array
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleKnightMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleKingMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleQueenMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossiblePawnMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleRookMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBishopMoves;


	// Possible moves array for check
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBishopMovesForCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleRookMovesForCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleQueenMovesForCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossiblePawnMovesForCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleKingMovesForCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleKnightMovesForCheck;


	// Illegal moves array
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> IllegalKingMoveDueToCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> IllegalQueenMoveDueToCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> IllegalKnightMoveDueToCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> IllegalBishopMoveDueToCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> IllegalPawnMoveDueToCheck;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> IllegalRookMoveDueToCheck;
	
	// In-game actors array (by color). Filled by CalculatePossibleMoves()
	UPROPERTY(EditDefaultsOnly)
		TArray<ABasePiece*> Actors;

	// Register move
	UPROPERTY(EditDefaultsOnly)
		FString Message;

	// Is check? Defined in *Piece*PossibleMoves() functions
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool Check = false;
	// Used by register for kill notation "x"
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsKillMove = false;

	// Variables used by IsCheckMate() to define how many pieces are in game
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int PawnIsInGame = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int QueenIsInGame = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int BishopIsInGame = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int KnightIsInGame = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int RookIsInGame = 0;

	// Variables used by IsCheckMate() to define how many in game pieces
	//(information given by *Piece*IsInGame Variables) have no moves left
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int NoPawnMovesLeftIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int NoQueenMovesLeftIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int NoBishopMovesLeftIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int NoKnightMovesLeftIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int NoRookMovesLeftIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int RegisterMovesCounter = 0;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Possible moves functions
	void KnightPossibleMoves(FVector KnightLocation, ETileOwner EnemyColor);
	void KingPossibleMoves(FVector KingLocation, ETileOwner EnemyColor);
	void PawnPossibleMoves(FVector PawnLocation, ETileOwner EnemyColor);
	void RookPossibleMoves(FVector RookLocation, ETileOwner EnemyColor);
	void BishopPossibleMoves(FVector BishopLocation, ETileOwner EnemyColor);
	void QueenPossibleMoves(FVector QueenLocation, ETileOwner EnemyColor);
	void CalculateBishopMoves(FVector BishopLocation, FVector2D Direction, ETileOwner EnemyColor);
	void CalculateRookMoves(FVector RookLocation, FVector2D Direction, ETileOwner EnemyColor);
	void CalculateQueenMoves(FVector QueenLocation, FVector2D Direction, ETileOwner EnemyColor);

	// Called at the start of a turn. Calculate if the friendcolor king is in check
	void IsCheckKing(ETileOwner FriendColor, ETileOwner EnemyColor);
	// Calculate all the chessboard check states (of friend color king) and calculate the illegal moves 
	void SimulatePossibleMoves(ETileOwner FriendColor, ETileOwner EnemyColor);
	// Calculate all possible moves by color
	void CalculatePossibleMoves(ETileOwner FriendColor, ETileOwner EnemyColor);
	// Simulate all the moves without moving in-game pieces
	void MoveSimulation(FVector SelectedActorLocation, FVector2D SelectedMovePosition, ETileOwner FriendColor, ETileOwner EnemyColor, ABasePiece* SelectedActor);
	// Called at the start of a turn. Calculate if the friend color king is in checkmate. If yes game ends.
	void IsCheckMate(ETileOwner FriendColor, ETileOwner EnemyColor);

	// Called after piece move. Convert game moves in strings with register notation
	void RegisterMoveConverter(FVector2D MovePosition, FVector2D OldPosition, ABasePiece* BasePieceActor, ETileOwner EnemyColor);
	// Converts x, y boards positions into letter
	FString NumberToCharConverter(FVector2D MovePosition);
	// Calculate check for register notation "+"
	void CalculateCheckForRegister(FVector2D MovePosition, ETileOwner EnemyColor, ABasePiece* PossiblePiece);
	
};
