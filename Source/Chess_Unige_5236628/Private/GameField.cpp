// Fill out your copyright notice in the Description page of Project Settings.


#include "GameField.h"
#include "BasePiece.h"
#include "CHS_GameMode.h"
#include "CHS_HumanPlayer.h"
#include "EngineUtils.h"



// Sets default values
AGameField::AGameField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Size of the field (3x3)
	Size = 3;
	// Tile dimension
	TileSize = 120;
	// Tile padding dimension
	CellPadding = 20;

	GameInstance = Cast<UCHS_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Normalized tilepadding
	NormalizedCellPadding = FMath::RoundToDouble(((TileSize + CellPadding) / TileSize) * 100) / 100;
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
	
}

void AGameField::ResetField()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	// Disable player input
	PlayerController->DisableInput(PlayerController);
	// Set timer
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{

		ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());
		// Set at 1 so human player starts
		GameMode->IsMyTurn = 1;
		// Iterate over TileMap
		for (auto& Pair : TileMap)
		{
			ATile* Tile = Pair.Value;
			// Reset tiles status for safety
			Tile->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
			// Destroy old tiles
			Tile->Destroy();
		}
		// Iterate over BasePieceMap
		for (auto& Pair : BasePieceMap)
		{
			ABasePiece* Piece = Pair.Value;
			// destroy old chess pieces
			Piece->Destroy();
		}
		// Iterate over PiecesForReplay array
		for (ABasePiece* Piece : GameInstance->PiecesForReplay)
		{
			Piece->Destroy();
		}
		// Iterate over PiecesAfterPromo array
		for (ABasePiece* Piece : GameInstance->PieceAfterPromo)
		{
			Piece->Destroy();
		}
		// Iterate over an array of structs
		for (int i = 0; i < GameInstance->DestroyedPieceArray.Num(); i++)
		{
			GameInstance->DestroyedPieceArray[i].Piece->Destroy();
		}
		// Iterate over an array of structs
		for (int i = 0; i < GameInstance->PromotedPieceArray.Num(); i++)
		{
			GameInstance->PromotedPieceArray[i].Piece->Destroy();
		}
		
		// Empty gamefield data 
		BasePieceMap.Empty();
		TileMap.Empty();
		BasePieceArray.Empty();
		TileArray.Empty();
		
		// Reset in-game messages
		FString reset = TEXT(" ");
		GameInstance->SetRegisterMove(reset);
		GameInstance->SetTurnMessage(reset);

		// Empty register data
		GameInstance->Moves.Empty();
		GameInstance->MovesForReplay.Empty();
		GameInstance->PiecesForReplay.Empty();
		GameInstance->DestroyedPieceArray.Empty();
		GameInstance->PromotedPieceArray.Empty();
		GameInstance->PieceAfterPromo.Empty();

		// Index reset
		GameInstance->DestroyedPieceArrayIndexCounter = 0;
		GameInstance->IsGameFinished = false;

		// Generate new field
		GenerateField();
	
		// Reset event
		OnResetEvent.Broadcast();

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

		// Enable again human input
		PlayerController->EnableInput(PlayerController);

	}, 0.5, false);
	
}

void AGameField::GenerateField()
{
	// TileMaterial variable declaration
	UMaterialInterface* TileMaterial;

	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			// Tile spawn
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);

			// Get last tile xyz coordinates
			FVector LocationXYZCoordinate = Obj -> GetActorLocation();

			// Tile vector xy coordinates
			FVector2D LocationXYCoordinate(LocationXYZCoordinate);

			// Add new tile to tilearray
			TileArray.Add(Obj);

			// TileMap key uses XY coordinates. Not relative
			TileMap.Add(LocationXYCoordinate, Obj);

			// Set tile status & owner
			Obj->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);

			int32 Color=NULL;

			// Set TileMaterial using chessboard pattern
			if ((x + y) % 2 == 0)
			{
				TileMaterial = Obj->BlackMaterial;
			}
			else
			{
				TileMaterial = Obj->WhiteMaterial;
			}
			
			// Apply TileMaterial to the tile using method SetTileMaterial
			Obj->SetTileMaterial(0,TileMaterial);

			// Chess Pieces spawn in their starting position
			if ((x == 0 && y==2)||(x==0 && y==5))
			{
				// Color = 1 = WHITE; 
				//WHITE
				Color = 1;
				// Bishop spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, BishopClass);
			}
			if ((x == 7 && y == 2) || (x == 7 && y == 5))
			{
				// Color = 2 = BLACK;
				//BLACK
				Color = 2;
				// Bishop spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, BishopClass);
			}
			if ((-1 < y && y < 8) && x == 1)
			{
				//WHITE
				Color = 1;
				// Pawn spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, PawnClass);
			}
			if ((-1 < y && y < 8) && x == 6)
			{
				//BLACK
				Color = 2;
				// Pawn spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, PawnClass);
			}
			if ((x == 0 && y == 0) || (x == 0 && y == 7))
			{
				//WHITE
				Color = 1;
				// Rook spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, RookClass);
			}
			if ((x == 7 && y == 0) || (x == 7 && y == 7))
			{
				//BLACK
				Color = 2;
				// Rook spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, RookClass);
			}
			if ((x == 0 && y == 1) || (x == 0 && y == 6))
			{
				//WHITE
				Color = 1;
				// Knight spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, KnightClass);
			}
			if ((x == 7 && y == 1) || (x == 7 && y == 6))
			{
				//BLACK
				Color = 2;
				// Knight spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, KnightClass);
			}
			if ((x == 0 && y == 4))
			{
				//WHITE
				Color = 1;
				// King spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, KingClass);
			}
			if ((x == 7 && y == 4))
			{
				//BLACK
				Color = 2;
				// King spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, KingClass);
			}
			if ((x == 0 && y == 3))
			{
				//WHITE
				Color = 1;
				// Queen spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, QueenClass);
			}
			if ((x == 7 && y == 3))
			{
				//BLACK
				Color = 2;
				// Queen spawn
				SpawnChessPiece(x, y, Location, TileScale, Color, 0, QueenClass);
			}
			// Set 
			if(Color==1){
				Obj->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
			}
			if (Color == 2) {
				Obj->SetTileStatus(ETileOwner::BLACK, ETileStatus::OCCUPIED);
			}
		}
		
	}
	// Saves pieces starting positions into a map for replay
	GameInstance->PiecesStartingPosition = BasePieceMap;
}

// Called during GenerateField(), or after a PawnPromotion, or for Replay
void AGameField::SpawnChessPiece(int32 x, int32 y, FVector Location, float TileScale, int32 Color, int32 IsAPromotedPiece, TSubclassOf<ABasePiece> PieceClass)
{
	// Spawn piece
	ABasePiece* ChessPiece = GetWorld()->SpawnActor<ABasePiece>(PieceClass, FVector(Location.X, Location.Y, Location.Z + 10), FRotator::ZeroRotator);
	ChessPiece->SetActorScale3D(FVector(TileScale, TileScale, 0.01));
	ChessPiece->SetBasePieceGridPosition(x, y);

	// Add piece to array and map
	BasePieceArray.Add(ChessPiece);
	BasePieceMap.Add(FVector2D(x, y), ChessPiece);

	// Set Piece material
	SetPieceColor(Color, ChessPiece);

	// If=1 the spawned piece is promoted
	if (IsAPromotedPiece == 1)
	{
		// Add piece to PieceAfterPromo array for replay
		GameInstance->PieceAfterPromo.Add(ChessPiece);
	}
}


// Set chess pieces' materials
template <typename T>
void AGameField::SetPieceColor(int32 Color, T* ChessPiece)
{
	// BasePiece Material variable declaration
	UMaterialInterface* BasePieceMaterial;

	// 1==WHITE
	if (Color == 1)
	{
		// Set White Material Interface
		BasePieceMaterial = ChessPiece->WhiteMaterial;
		ChessPiece->SetBasePieceMaterial(0, BasePieceMaterial);
		// Set PieceColor ENUM WHITE
		ChessPiece->SetPieceColor(EPieceColor::WHITE);
		
	}
	else
	{
		BasePieceMaterial = ChessPiece->BlackMaterial;
		ChessPiece->SetBasePieceMaterial(0, BasePieceMaterial);
		ChessPiece->SetPieceColor(EPieceColor::BLACK);
	}


}

// Called by Promotion widget blueprint or by AI classes
void AGameField::PawnPromotion(ABasePiece* Pawn, int32 Color, FString NewPiece)
{
	// Get pawn position
	FVector PawnPosition = Pawn->GetActorLocation();
	// Normalize
	FVector2D PawnPositionNormalized(PawnPosition.X / 120, PawnPosition.Y / 120);
	
	// Set pawn hidden 
	Pawn->SetActorHiddenInGame(true);
	// Set pawn without collision
	Pawn->SetActorEnableCollision(false);

	// Create new promoted piece struct for replay
	FPromotedPiece NewPromotedPiece;
	NewPromotedPiece.Piece = Pawn;
	NewPromotedPiece.Position = PawnPositionNormalized;
	// Use the same index used for destroyed piece, it is equivalent to a turn counter
	NewPromotedPiece.TurnCounter = GameInstance->DestroyedPieceArrayIndexCounter;
	// Add new struct to an array of struct
	GameInstance->PromotedPieceArray.Add(NewPromotedPiece);
	
	// Set pawn location away from game field to untrigger the in-game triggerbox
	Pawn->SetActorLocation(FVector(1700, 1700, 1700));
	// Remove pawn from Pieces map
	BasePieceMap.Remove(PawnPositionNormalized);

	// Create the promoted piece 
	if (NewPiece == "Queen")
	{
		SpawnChessPiece(PawnPositionNormalized.X, PawnPositionNormalized.Y, PawnPosition, 1.2, Color, 1, QueenClass);
	}
	else if (NewPiece == "Rook")
	{
		SpawnChessPiece(PawnPositionNormalized.X, PawnPositionNormalized.Y, PawnPosition, 1.2, Color, 1, RookClass);
	}
	else if (NewPiece == "Knight")
	{
		SpawnChessPiece(PawnPositionNormalized.X, PawnPositionNormalized.Y, PawnPosition, 1.2, Color, 1, KnightClass);
	}
	else if (NewPiece == "Bishop")
	{
		SpawnChessPiece(PawnPositionNormalized.X, PawnPositionNormalized.Y, PawnPosition, 1.2, Color, 1, BishopClass);
	}
	// If Human called promotion
	if (Color == 1)
	{
		// Update turn counter index
		GameInstance->DestroyedPieceArrayIndexCounter++;

		ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
		// End Human Turn
		GameMode->EndHumanTurn();
	}
	else
	{	// Update turn counter index
		GameInstance->DestroyedPieceArrayIndexCounter++;
	}

}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}




