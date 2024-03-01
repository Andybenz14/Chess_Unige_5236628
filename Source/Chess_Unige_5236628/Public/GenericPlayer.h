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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	

	UPROPERTY(EditDefaultsOnly)
		TArray<ABasePiece*> Actors;


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


	void IsCheckKing(ETileOwner FriendColor, ETileOwner EnemyColor);
	void SimulatePossibleMoves(ETileOwner FriendColor, ETileOwner EnemyColor);
	void CalculatePossibleMoves(ETileOwner FriendColor, ETileOwner EnemyColor);
	void MoveSimulation(FVector SelectedActorLocation, FVector2D SelectedMovePosition, ETileOwner FriendColor, ETileOwner EnemyColor, ABasePiece* SelectedActor);
	void IsCheckMate(ETileOwner FriendColor, ETileOwner EnemyColor);


	void RegisterMoveConverter(FVector2D MovePosition, FVector2D OldPosition, ABasePiece* BasePieceActor, ETileOwner EnemyColor);
	FString NumberToCharConverter(FVector2D MovePosition);
	void CalculateCheckForRegister(FVector2D MovePosition, ETileOwner EnemyColor, ABasePiece* PossiblePiece);

	UPROPERTY(EditDefaultsOnly)
	FString Message;

	bool Check = false;
	bool IsKillMove = false;
	int PawnIsInGame = 0;
	int QueenIsInGame = 0;
	int BishopIsInGame = 0;
	int KnightIsInGame = 0;
	int RookIsInGame = 0;
	int RegisterMovesCounter = 0;

	
	
};
