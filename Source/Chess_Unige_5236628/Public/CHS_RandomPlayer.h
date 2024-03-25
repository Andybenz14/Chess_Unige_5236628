// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GenericPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "CHS_RandomPlayer.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API ACHS_RandomPlayer : public AGenericPlayer
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

	// On Turn
	virtual void OnTurn();

	// Move piece into a new position
	void MoveBaseBlackPiece(ABasePiece*, FVector OldLocation, FVector NewLocation);

	// Set white killed piece hidden
	void SetKilledPieceHidden(FVector2D NormalizedPosition);

	// Array of black movable actors 
	UPROPERTY(EditDefaultsOnly)
		TArray<ABasePiece*> BlackMovableActors;
};
