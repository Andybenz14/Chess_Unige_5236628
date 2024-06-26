// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "BasePiece.h"
#include "PawnChess.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include "Bishop.h"
#include "Rook.h"
#include "CHS_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"


// macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);

UCLASS()
class CHESS_UNIGE_5236628_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:	

	// Keeps track of tiles
	UPROPERTY(Transient)
		TArray<ATile*> TileArray;

	// Keeps track of pieces
	UPROPERTY(Transient)
		TArray<ABasePiece*> BasePieceArray;

	// Given a position returns a tile
	UPROPERTY(Transient)
		TMap<FVector2D, ATile*> TileMap;

	// Given a position returns a piece
	UPROPERTY(Transient)
	    TMap<FVector2D, ABasePiece*> BasePieceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float NormalizedCellPadding;

	static const int32 NOT_ASSIGNED = -1;

	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
		FOnReset OnResetEvent;

	// Size of field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Size;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ATile> TileClass;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABishop> BishopClass;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APawnChess> PawnClass;
	
	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ARook> RookClass;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AKing> KingClass;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AQueen> QueenClass;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AKnight> KnightClass; 

	// Tile padding dimension
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CellPadding;

	// Tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TileSize;

	// GameIstance
	UPROPERTY(EditDefaultsOnly)
	UCHS_GameInstance* GameInstance;

	// Sets default values for this actor's properties
	AGameField();

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Remove all signs from the field
	UFUNCTION(BlueprintCallable)
		void ResetField();

	// Generate an empty game field
	void GenerateField();

	// Return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// Spawn in game chess pieces
	void SpawnChessPiece(int32 x, int32 y, FVector Location, float TileScale, int32 Color, int32 IsAPromotedPiece, TSubclassOf<ABasePiece> PieceClass);

	// Set chess pieces materials (called at pieces spawn)
	template <typename T>
	void SetPieceColor(int32 Color, T* ChessPiece);
	
	// Change pawn actor with a new piece given pawn pointer, a color (1==WHITE, 2==BLACK) and the type of the new piece
	UFUNCTION(BlueprintCallable)
	void PawnPromotion(ABasePiece* Pawn, int32 Color, FString NewPiece);

};
