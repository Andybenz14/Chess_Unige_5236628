// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CHS_PlayerInterface.h"
#include "CHS_GameInstance.h"
#include "CHS_GameMode.h"
#include "GameField.h"
#include "Kismet/GameplayStatics.h"
#include "CHS_RandomPlayer.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API ACHS_RandomPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACHS_RandomPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Array of black pieces
	UPROPERTY(Transient)
	TArray<ABasePiece*> BlackActor;

	// Position of a black piece
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector BlackActorLocation;

	// BasePiece Actor
	UPROPERTY(EditDefaultsOnly)
		ABasePiece* RandomSelectedActor;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn();

	// Array of the positions that can be occupied by the black knight
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBKnightMoves;

	// Array of the positions that can be occupied by the black king 
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBKingMoves;

	// Array of the positions that can be occupied by the black queen 
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBQueenMoves;

	// Array of the positions that can be occupied by the black pawn 
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBPawnMoves;

	// Array of the positions that can be occupied by the black rook 
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBRookMoves;

	// Array of the positions that can be occupied by the black bishop 
	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBBishopMoves;

	// Array of the positions that can be occupied by the black  
	UPROPERTY(EditDefaultsOnly)
		TArray<ABasePiece*> BlackMovableActors;

	// Calculate the positions that can be occupied by the black knight from his actual position
	void BKnightPossibleMoves(FVector KnightLocation);

	// Calculate the positions that can be occupied by the black king from his actual position
	void BKingPossibleMoves(FVector KingLocation);
	
	// Calculate the positions that can be occupied by the black pawn from his actual position
	void BPawnPossibleMoves(FVector PawnLocation);

	// Calculate the positions that can be occupied by the black rook from his actual position
	void BRookPossibleMoves(FVector RookLocation);

	// Calculate the positions that can be occupied by the black bishop from his actual position
	void BBishopPossibleMoves(FVector BishopLocation);

	// Calculate the positions that can be occupied by the black queen from his actual position
	void BQueenPossibleMoves(FVector QueenLocation);

	// Move piece into a new position
	void MoveBaseBlackPiece(ABasePiece*, FVector OldLocation, FVector NewLocation);

};
