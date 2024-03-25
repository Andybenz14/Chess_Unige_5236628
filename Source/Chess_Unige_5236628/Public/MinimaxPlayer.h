// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlayer.h"
#include "GameFramework/Pawn.h"
#include "MinimaxPlayer.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API AMinimaxPlayer : public AGenericPlayer
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinimaxPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Minimax on turn
	virtual void OnTurn();

	// Best value
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 V;

	// Gives a score of chessboard state
	int32 Evaluate();
	// Gives a score to a promotion move
	int32 EvaluatePawnPromotion(ABasePiece* Actor, int32 Color);
	// Gives a score to opening movements
	int32 EvaluateStartingGameOpening(ABasePiece* Actor, int32 Color);
	// Find white king location to attack him
	FVector2D FindKingLocation();
	// Minimax main function
	int32 AlfaBetaMinimax(int32 Depth, bool IsMax, int32 Alfa, int32 Beta, FVector2D& BestMove, ABasePiece*& BestActor);
	// Simulate a move in the TMaps without moving pieces in-game. For minimax
	void MoveSimulation(FVector SelectedActorLocation, FVector2D SelectedMovePosition, ETileOwner FriendColor, ETileOwner EnemyColor, ABasePiece* SelectedActor, int32 Depth, int32 Alfa, int32 Beta, FVector2D& BestMove, ABasePiece*& BestActor);
	// Move piece after minimax choose the best move
	void MoveBaseBlackPiece(ABasePiece*, FVector OldLocation, FVector NewLocation);
	// Set killed piece hidden
	void SetKilledPieceHidden(FVector2D NormalizedPosition);
;   // Reset tilemap status and owner after minimax. For safety 
	void TileMapReset();
	
};
