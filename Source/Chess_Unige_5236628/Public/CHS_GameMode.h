// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameField.h"
#include "BasePiece.h"
#include "PawnChess.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "CHS_HumanPlayer.h"
#include "GameFramework/GameModeBase.h"
#include "CHS_GameMode.generated.h"

class AActor;

struct FPosition;

/**
 * 
 */
UCLASS()
class CHESS_UNIGE_5236628_API ACHS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGameField> GameFieldClass;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABishop> BishopClass;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AQueen> QueenClass;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APawnChess> PawnChessClass;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AKing> KingClass;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AKnight> KnightClass;
	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ARook> RookClass;

	// Field size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 FieldSize;

	// Turn counter
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 IsMyTurn;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		AGameField* GField;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		ABishop* Bishop;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		APawnChess* PawnChess;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		AQueen* Queen;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		AKing* King;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		AKnight* Knight;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		ARook* Rook;
	
	// End human turn
	void EndHumanTurn();

	// End AI turn
	void EndAITurn();

	// Bool game finished
	bool IsGameFinished() const;

	ACHS_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
