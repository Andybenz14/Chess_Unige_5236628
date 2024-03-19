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

	virtual void OnTurn();

	int32 V;

	int32 Evaluate();

	int32 EvaluateCenterControl(ABasePiece* Actor);

	int32 EvaluateKingDefence(ABasePiece* Actor, int32 Color);
	
	int32 AlfaBetaMinimax(int32 Depth, bool IsMax, int32 Alfa, int32 Beta, FVector2D& BestMove, ABasePiece*& BestActor);

	void MoveSimulation(FVector SelectedActorLocation, FVector2D SelectedMovePosition, ETileOwner FriendColor, ETileOwner EnemyColor, ABasePiece* SelectedActor, int32 Depth, int32 Alfa, int32 Beta, FVector2D& BestMove, ABasePiece*& BestActor);
	
	void MoveBaseBlackPiece(ABasePiece*, FVector OldLocation, FVector NewLocation);

	void SetKilledPieceHidden(FVector2D NormalizedPosition);
; 
	void TileMapReset();
	
	
};
