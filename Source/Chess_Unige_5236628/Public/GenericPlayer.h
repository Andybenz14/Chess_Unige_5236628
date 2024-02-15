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
		TArray<FVector2D> AllWhitePossibleMoves;


	bool Check = false;


	// Possible moves functions
	void KnightPossibleMoves(FVector KnightLocation, ETileOwner EnemyColor);
	void KingPossibleMoves(FVector KingLocation, ETileOwner EnemyColor);
	void PawnPossibleMoves(FVector PawnLocation, ETileOwner EnemyColor);
	void RookPossibleMoves(FVector RookLocation, ETileOwner EnemyColor);
	void BishopPossibleMoves(FVector BishopLocation, ETileOwner EnemyColor);
	void QueenPossibleMoves(FVector QueenLocation, ETileOwner EnemyColor);

	void CheckKing(ETileOwner EnemyColor, ETileOwner FriendColor);
};
