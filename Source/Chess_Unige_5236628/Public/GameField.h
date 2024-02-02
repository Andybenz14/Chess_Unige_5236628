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
#include "GameFramework/Actor.h"
#include "GameField.generated.h"


// macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);

UCLASS()
class CHESS_UNIGE_5236628_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:	
	// keeps track of tiles
	UPROPERTY(Transient)
		TArray<ATile*> TileArray;

	// keeps track of pieces
	UPROPERTY(Transient)
		TArray<ABasePiece*> BasePieceArray;

	//given a position returns a tile
	UPROPERTY(Transient)
		TMap<FVector2D, ATile*> TileMap;

	//given a position returns a piece
	UPROPERTY(Transient)
		TMap<FVector2D, ABasePiece*> BasePieceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float NormalizedCellPadding;

	static const int32 NOT_ASSIGNED = -1;

	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
		FOnReset OnResetEvent;

	// size of field
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

	// tile padding dimension
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CellPadding;

	// tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TileSize;


	// Sets default values for this actor's properties
	AGameField();

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// remove all signs from the field
	UFUNCTION(BlueprintCallable)
		void ResetField();

	// generate an empty game field
	void GenerateField();

	// return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<ATile*>& GetTileArray();

	// return the array of tile pointers
	TArray<ABasePiece*>& GetBasePieceArray();

	// return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

	// checking if is a valid field position
	inline bool IsValidPosition(const FVector2D Position) const;

	// get a line given a begin and end positions
	//TArray<int32> GetLine(const FVector2D Begin, const FVector2D End) const;

	// spawn bishop 
	void SpawnBishop(int32 x, int32 y, FVector Location, float TileScale, int32 Color);

	// spawn pawn
	void SpawnPawn(int32 x, int32 y, FVector Location, float TileScale, int32 Color);

	// spawn rook
	void SpawnRook(int32 x, int32 y, FVector Location, float TileScale, int32 Color);

	// spawn king
	void SpawnKing(int32 x, int32 y, FVector Location, float TileScale, int32 Color);
	
	// spawn queen
	void SpawnQueen(int32 x, int32 y, FVector Location, float TileScale, int32 Color);
	
	// spawn knight
	void SpawnKnight(int32 x, int32 y, FVector Location, float TileScale, int32 Color);

	// set chess pieces materials (called at spawn)
	template <typename T>
	void SetPieceColor(int32 Color, T* ChessPiece);
	


	//public:	
	//	// Called every frame
	//	virtual void Tick(float DeltaTime) override;

};
