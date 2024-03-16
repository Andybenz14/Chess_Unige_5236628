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

	int32 Evaluate();
	
	int32 AlfaBetaMinimax(const TMap<FVector2D, ABasePiece*>& State, int32 Depth, int32 Alfa, int32 Beta, bool IsMax);

	TMap<FVector2D, ABasePiece*> UpdateInGamePiecesMapForMinimaxSimulation();
	

	int32 NodeCounter = 0;
	int32 V;

	UPROPERTY(Transient)
		TMap<FVector2D, ABasePiece*> PiecesMapForMinimax;
};
