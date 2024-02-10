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

	UPROPERTY(Transient)
	TArray<ABasePiece*> BlackActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector BlackActorLocation;

	UPROPERTY(EditDefaultsOnly)
		ABasePiece* RandomSelectedActor;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn();

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBKnightMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBKingMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBQueenMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBPawnMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBRookMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleBBishopMoves;

	UPROPERTY(EditDefaultsOnly)
		TArray<ABasePiece*> BlackMovableActors;

	void BKnightPossibleMoves(FVector KnightLocation);
	void BKingPossibleMoves(FVector KingLocation);
	void BPawnPossibleMoves(FVector PawnLocation);
	void BRookPossibleMoves(FVector RookLocation);
	void BBishopPossibleMoves(FVector BishopLocation);
	void BQueenPossibleMoves(FVector QueenLocation);

	void MoveBaseBlackPiece(ABasePiece*, FVector OldLocation, FVector NewLocation);

};
