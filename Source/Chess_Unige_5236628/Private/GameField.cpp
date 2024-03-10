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
	// size of the field (3x3)
	Size = 3;
	// tile dimension
	TileSize = 120;
	// tile padding dimension
	CellPadding = 20;

	GameInstance = Cast<UCHS_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//normalized tilepadding
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

	PlayerController->DisableInput(PlayerController);

	//TODO NIENTE TIMER 
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->IsMyTurn = 1;

	for (auto& Pair : TileMap)
	{
		ATile* Tile = Pair.Value;
		Tile->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		Tile->Destroy();
	}
	for (auto& Pair : BasePieceMap)
	{
		ABasePiece* Piece = Pair.Value;
		Piece->Destroy();
	}
	for (ABasePiece* Piece : GameInstance->PiecesForReplay)
	{
		Piece->Destroy();
	}
	for (ABasePiece* Piece : GameInstance->PieceAfterPromo)
	{
		Piece->Destroy();
	}
	for (int i = 0; i < GameInstance->DestroyedPieceArray.Num(); i++)
	{
		GameInstance->DestroyedPieceArray[i].Piece->Destroy();
	}
	for (int i = 0; i < GameInstance->PromotedPieceArray.Num(); i++)
	{
		GameInstance->PromotedPieceArray[i].Piece->Destroy();
	}
	
	BasePieceMap.Empty();
	TileMap.Empty();
	BasePieceArray.Empty();
	TileArray.Empty();
	
	FString reset = TEXT(" ");
	GameInstance->SetRegisterMove(reset);
	GameInstance->SetTurnMessage(reset);
	GameInstance->Moves.Empty();
	GameInstance->MovesForReplay.Empty();
	GameInstance->PiecesForReplay.Empty();
	GameInstance->DestroyedPieceArray.Empty();
	GameInstance->PromotedPieceArray.Empty();
	GameInstance->PieceAfterPromo.Empty();
	GameInstance->DestroyedPieceArrayIndexCounter = 0;

	// Genera nuovamente il campo e le pedine
	GenerateField();
	
	// Invia l'evento di reset a tutti gli oggetti registrati
	OnResetEvent.Broadcast();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

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
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);

			// Get last tile xyz coordinates
			FVector LocationXYZCoordinate = Obj -> GetActorLocation();

			// Tile vector xy coordinates
			FVector2D LocationXYCoordinate(LocationXYZCoordinate);

			TileArray.Add(Obj);

			// TileMap key uses XY coordinates. Not relative
			TileMap.Add(LocationXYCoordinate, Obj);

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
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//WHITE
				Color = 1;
				SpawnBishop(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 7 && y == 2) || (x == 7 && y == 5))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//BLACK
				Color = 2;
				SpawnBishop(x, y, Location, TileScale, Color, 0);
			}
			if ((-1 < y && y < 8) && x == 1)
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//WHITE
				Color = 1;
				SpawnPawn(x, y, Location, TileScale, Color, 0);
			}
			if ((-1 < y && y < 8) && x == 6)
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//BLACK
				Color = 2;
				SpawnPawn(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 0 && y == 0) || (x == 0 && y == 7))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//WHITE
				Color = 1;
				SpawnRook(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 7 && y == 0) || (x == 7 && y == 7))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//BLACK
				Color = 2;
				SpawnRook(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 0 && y == 1) || (x == 0 && y == 6))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//WHITE
				Color = 1;
				SpawnKnight(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 7 && y == 1) || (x == 7 && y == 6))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//BLACK
				Color = 2;
				SpawnKnight(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 0 && y == 4))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//WHITE
				Color = 1;
				SpawnKing(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 7 && y == 4))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//BLACK
				Color = 2;
				SpawnKing(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 0 && y == 3))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//WHITE
				Color = 1;
				SpawnQueen(x, y, Location, TileScale, Color, 0);
			}
			if ((x == 7 && y == 3))
			{
				// Color = 1 = WHITE; Color = 2 = BLACK;
				//BLACK
				Color = 2;
				SpawnQueen(x, y, Location, TileScale, Color, 0);
			}

			if(Color==1){
				Obj->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
			}
			if (Color == 2) {
				Obj->SetTileStatus(ETileOwner::BLACK, ETileStatus::OCCUPIED);
			}
		}
		
	}
	
	GameInstance->PiecesStartingPosition = BasePieceMap;
}


// Spawn function for Bishop 
void AGameField::SpawnBishop(int32 x, int32 y, FVector Location, float TileScale, int32 Color, int32 IsAPromotedPiece)
{
	ABishop* ChessPiece = GetWorld()->SpawnActor<ABishop>(BishopClass, FVector(Location.X, Location.Y, Location.Z + 10), FRotator::ZeroRotator);
	ChessPiece->SetActorScale3D(FVector(TileScale, TileScale, 0.01));
	ChessPiece->SetBasePieceGridPosition(x, y);
	BasePieceArray.Add(ChessPiece);
	BasePieceMap.Add(FVector2D(x, y), ChessPiece);
	SetPieceColor(Color, ChessPiece);

	if (IsAPromotedPiece == 1)
	{
		GameInstance->PieceAfterPromo.Add(ChessPiece);
	}
	
}

// Spawn function for Pawn
void AGameField::SpawnPawn(int32 x, int32 y, FVector Location, float TileScale, int32 Color, int32 IsAPromotedPiece)
{
	APawnChess* ChessPiece = GetWorld()->SpawnActor<APawnChess>(PawnClass, FVector(Location.X, Location.Y, Location.Z + 10), FRotator::ZeroRotator);
	ChessPiece->SetActorScale3D(FVector(TileScale, TileScale, 0.01));
	ChessPiece->SetBasePieceGridPosition(x, y);
	BasePieceArray.Add(ChessPiece);
	BasePieceMap.Add(FVector2D(x, y), ChessPiece);
	SetPieceColor(Color, ChessPiece);

	if (IsAPromotedPiece == 1)
	{
		GameInstance->PieceAfterPromo.Add(ChessPiece);
	}

}

// Spawn function for Knight
void AGameField::SpawnKnight(int32 x, int32 y, FVector Location, float TileScale, int32 Color, int32 IsAPromotedPiece)
{
	AKnight* ChessPiece = GetWorld()->SpawnActor<AKnight>(KnightClass, FVector(Location.X, Location.Y, Location.Z + 10), FRotator::ZeroRotator);
	ChessPiece->SetActorScale3D(FVector(TileScale, TileScale, 0.01));
	ChessPiece->SetBasePieceGridPosition(x, y);
	BasePieceArray.Add(ChessPiece);
	BasePieceMap.Add(FVector2D(x, y), ChessPiece);
	SetPieceColor(Color, ChessPiece);

	if (IsAPromotedPiece == 1)
	{
		GameInstance->PieceAfterPromo.Add(ChessPiece);
	}

}

// Spawn function for King
void AGameField::SpawnKing(int32 x, int32 y, FVector Location, float TileScale, int32 Color, int32 IsAPromotedPiece)
{
	AKing* ChessPiece = GetWorld()->SpawnActor<AKing>(KingClass, FVector(Location.X, Location.Y, Location.Z + 10), FRotator::ZeroRotator);
	ChessPiece->SetActorScale3D(FVector(TileScale, TileScale, 0.01));
	ChessPiece->SetBasePieceGridPosition(x, y);
	BasePieceArray.Add(ChessPiece);
	BasePieceMap.Add(FVector2D(x, y), ChessPiece);
	SetPieceColor(Color, ChessPiece);

	if (IsAPromotedPiece == 1)
	{
		GameInstance->PieceAfterPromo.Add(ChessPiece);
	}

}

// Spawn function for Queen
void AGameField::SpawnQueen(int32 x, int32 y, FVector Location, float TileScale, int32 Color, int32 IsAPromotedPiece)
{
	AQueen* ChessPiece = GetWorld()->SpawnActor<AQueen>(QueenClass, FVector(Location.X, Location.Y, Location.Z + 10), FRotator::ZeroRotator);
	ChessPiece->SetActorScale3D(FVector(TileScale, TileScale, 0.01));
	ChessPiece->SetBasePieceGridPosition(x, y);
	BasePieceArray.Add(ChessPiece);
	BasePieceMap.Add(FVector2D(x, y), ChessPiece);
	SetPieceColor(Color, ChessPiece);

	if (IsAPromotedPiece == 1)
	{
		GameInstance->PieceAfterPromo.Add(ChessPiece);
	}
	

}

//Spawn function for Rook
void AGameField::SpawnRook(int32 x, int32 y, FVector Location, float TileScale, int32 Color, int32 IsAPromotedPiece)
{
	ARook* ChessPiece = GetWorld()->SpawnActor<ARook>(RookClass, FVector(Location.X, Location.Y, Location.Z + 10), FRotator::ZeroRotator);
	ChessPiece->SetActorScale3D(FVector(TileScale, TileScale, 0.01));
	ChessPiece->SetBasePieceGridPosition(x, y);
	BasePieceArray.Add(ChessPiece);
	BasePieceMap.Add(FVector2D(x, y), ChessPiece);
	SetPieceColor(Color, ChessPiece);

	if (IsAPromotedPiece == 1)
	{
		GameInstance->PieceAfterPromo.Add(ChessPiece);
	}

}

// Set Material to chess pieces using color paremeter, defined in GenerateField using pieces positions
template <typename T>
void AGameField::SetPieceColor(int32 Color, T* ChessPiece)
{
	// BasePiece Material variable declaration
	UMaterialInterface* BasePieceMaterial;

	if (Color == 1)
	{
		BasePieceMaterial = ChessPiece->WhiteMaterial;
		ChessPiece->SetBasePieceMaterial(0, BasePieceMaterial);
		ChessPiece->SetPieceColor(EPieceColor::WHITE);
		
	}
	else
	{
		BasePieceMaterial = ChessPiece->BlackMaterial;
		ChessPiece->SetBasePieceMaterial(0, BasePieceMaterial);
		ChessPiece->SetPieceColor(EPieceColor::BLACK);
	}


}

void AGameField::PawnPromotion(ABasePiece* Pawn, int32 Color, FString NewPiece)
{
	FVector PawnPosition = Pawn->GetActorLocation();
	FVector2D PawnPositionNormalized(PawnPosition.X / 120, PawnPosition.Y / 120);
	
	Pawn->SetActorHiddenInGame(true);
	Pawn->SetActorEnableCollision(false);

	FPromotedPiece NewPromotedPiece;
	NewPromotedPiece.Piece = Pawn;
	NewPromotedPiece.Position = PawnPositionNormalized;
	NewPromotedPiece.TurnCounter = GameInstance->DestroyedPieceArrayIndexCounter;
	GameInstance->PromotedPieceArray.Add(NewPromotedPiece);
	
	
	Pawn->SetActorLocation(FVector(1700, 1700, 1700));
	BasePieceMap.Remove(PawnPositionNormalized);

	if (NewPiece == "Queen")

	{
		SpawnQueen(PawnPositionNormalized.X, PawnPositionNormalized.Y, PawnPosition, 1.2, Color, 1);

	}
	else if (NewPiece == "Rook")
	{
		SpawnRook(PawnPositionNormalized.X, PawnPositionNormalized.Y, PawnPosition, 1.2, Color, 1);
	}
	else if (NewPiece == "Knight")
	{
		SpawnKnight(PawnPositionNormalized.X, PawnPositionNormalized.Y, PawnPosition, 1.2, Color, 1);
	}
	else if (NewPiece == "Bishop")
	{
		SpawnBishop(PawnPositionNormalized.X, PawnPositionNormalized.Y, PawnPosition, 1.2, Color, 1);
	}

	if (Color == 1)
	{
		GameInstance->DestroyedPieceArrayIndexCounter++;
		ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
		
		GameMode->EndHumanTurn();
	}
	else
	{
		GameInstance->DestroyedPieceArrayIndexCounter++;
	}

}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

TArray<ABasePiece*>& AGameField::GetBasePieceArray()
{
	return BasePieceArray;
}

TMap<FVector2D, ATile*>& AGameField::GetTileMap() {

	return TileMap;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}


