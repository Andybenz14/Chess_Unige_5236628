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

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> AllPossibleMoves;

	bool Check = false;

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

	// Return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// Return the array of tile pointers
	TArray<ATile*>& GetTileArray();

	// Return the array of tile pointers
	TArray<ABasePiece*>& GetBasePieceArray();

	// Return the array of tile pointers
	TMap<FVector2D, ATile*>& GetTileMap();

	// Return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// Return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

	// Spawn bishop given position, a scale and a color
	void SpawnBishop(int32 x, int32 y, FVector Location, float TileScale, int32 Color);

	// Spawn pawn given position, a scale and a color
	void SpawnPawn(int32 x, int32 y, FVector Location, float TileScale, int32 Color);

	// Spawn rook given position, a scale and a color
	void SpawnRook(int32 x, int32 y, FVector Location, float TileScale, int32 Color);

	// Spawn king given position, a scale and a color
	void SpawnKing(int32 x, int32 y, FVector Location, float TileScale, int32 Color);
	
	// Spawn queen given position, a scale and a color
	void SpawnQueen(int32 x, int32 y, FVector Location, float TileScale, int32 Color);
	
	// Spawn knight given position, a scale and a color
	void SpawnKnight(int32 x, int32 y, FVector Location, float TileScale, int32 Color);

	// Set chess pieces materials (called at spawn)
	template <typename T>
	void SetPieceColor(int32 Color, T* ChessPiece);
	
	// Change pawn actor with a new piece given a pawn, a color (1==WHITE, 2==BLACK) and the type of the new piece
	UFUNCTION(BlueprintCallable)
	void PawnPromotion(ABasePiece* Pawn, int32 Color, FString NewPiece);

	void CheckKing(ETileOwner Color, ETileOwner EnemyColor);

	//public:	
	//	// Called every frame
	//	virtual void Tick(float DeltaTime) override;

};
