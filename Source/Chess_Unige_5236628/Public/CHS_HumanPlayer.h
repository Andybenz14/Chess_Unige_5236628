// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UW_PawnPromotionMenu.h"
#include "Camera/CameraComponent.h"
#include "CHS_PlayerInterface.h"
#include "CHS_HumanPlayer.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API ACHS_HumanPlayer : public APawn, public ICHS_PlayerInterface

{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACHS_HumanPlayer();

	// camera component attacched to player pawn
	UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// keeps track of turn
	bool IsMyTurn = true;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 ClickCounter = 0;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 TurnCounter = 0;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector ClickedActorLocation;

	UPROPERTY(EditDefaultsOnly)
		ABasePiece* BasePieceActor;

	UPROPERTY(EditDefaultsOnly)
		TMap<FVector2D, UMaterialInterface*> OriginalMaterials;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn();

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

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUW_PawnPromotionMenu> WidgetClass;
	// reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		UUW_PawnPromotionMenu* Promo;

	void KnightPossibleMoves(FVector KnightLocation);
	void KingPossibleMoves(FVector KingLocation);
	void PawnPossibleMoves(FVector PawnLocation);
	void RookPossibleMoves(FVector RookLocation);
	void BishopPossibleMoves(FVector BishopLocation);
	void QueenPossibleMoves(FVector QueenLocation);

	
	int32 IsPieceMoveValid(FVector2D Position, TArray<FVector2D> PossiblePieceMoves);

	void MoveBasePiece(ABasePiece*, FVector OldLocation, FVector NewLocation);

	void SetOriginalTileMaterial();

	// called on left mouse click (binding)
	UFUNCTION()
		void OnClick();

};
