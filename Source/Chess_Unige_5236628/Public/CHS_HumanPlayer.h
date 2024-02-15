// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePiece.h"
#include "Camera/CameraComponent.h"
#include "CHS_PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "CHS_HumanPlayer.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API ACHS_HumanPlayer : public APawn, public ICHS_PlayerInterface

{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACHS_HumanPlayer();

	// Camera component attacched to player pawn
	UCameraComponent* Camera;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Keeps track of turn
	bool IsMyTurn = true;

	// Click counter
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 ClickCounter = 0;

	// Turn counter
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 TurnCounter = 0;

	// Store clicked actor location
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector ClickedActorLocation;

	// BasePieceActor pointer
	UPROPERTY(EditDefaultsOnly)
		ABasePiece* BasePieceActor;

	// Map to save spawn tile materials before changing to show hint moves
	UPROPERTY(EditDefaultsOnly)
		TMap<FVector2D, UMaterialInterface*> OriginalMaterials;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when it's human turn
	virtual void OnTurn();

<<<<<<< HEAD
=======
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

	// Possible moves functions
	void KnightPossibleMoves(FVector KnightLocation);
	void KingPossibleMoves(FVector KingLocation);
	void PawnPossibleMoves(FVector PawnLocation);
	void RookPossibleMoves(FVector RookLocation);
	void BishopPossibleMoves(FVector BishopLocation);
	void QueenPossibleMoves(FVector QueenLocation);

>>>>>>> parent of 483c1e1 (Check starting implementatition)
	// Check if the clicked tile/black actor position is a legit move
	int32 IsPieceMoveValid(FVector2D Position, TArray<FVector2D> PossiblePieceMoves);

	// Move actor into new location
	void MoveBasePiece(ABasePiece*, FVector OldLocation, FVector NewLocation);

	// Set spawn tile materials
	void SetOriginalTileMaterial();

	// Called on left mouse click (binding)
	UFUNCTION()
		void OnClick();

	void ApplyPossibleMovesMaterials(const TArray<FVector2D>& PossibleMoves);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

	
=======
>>>>>>> parent of 483c1e1 (Check starting implementatition)
=======
>>>>>>> parent of 483c1e1 (Check starting implementatition)
=======
>>>>>>> parent of 483c1e1 (Check starting implementatition)
	
};
