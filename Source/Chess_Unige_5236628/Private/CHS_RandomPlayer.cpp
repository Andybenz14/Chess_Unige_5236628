// Fill out your copyright notice in the Description page of Project Settings.

#include "CHS_RandomPlayer.h"



// Sets default values
ACHS_RandomPlayer::ACHS_RandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACHS_RandomPlayer::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ACHS_RandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACHS_RandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Function called when human player turn ends 
void ACHS_RandomPlayer::OnTurn()
{
	// Timer
	FTimerHandle TimerHandle;

	// Set Timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		// Initialize an array of black pieces
		TArray<ABasePiece*> BlackActors;

		// Empty the array of black pieces
		BlackActors.Empty();

		// Empty the array of black movable pieces
		BlackMovableActors.Empty();

		// GameMode pointer
		ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	
		// Iterate on TileArray to find tiles owned by black pieces
		for (const auto& BlackTile : GameMode->GField->TileArray)
		{
			// Tile owner must be BLACK
			if (BlackTile->GetOwner() == ETileOwner::BLACK)
			{
				// Get tile location
				BlackActorLocation = BlackTile->GetActorLocation();

				// 2D black piece location (same as the tile position)
				FVector2D BlackPieceLocation2d(BlackActorLocation);

				// Normalize
				BlackPieceLocation2d.X = BlackPieceLocation2d.X / 120;
				BlackPieceLocation2d.Y = BlackPieceLocation2d.Y / 120;
			
				// Check if BasePieceMap contains the black piece by his 2d location
				if (GameMode->GField->BasePieceMap.Contains(BlackPieceLocation2d))
				{
					// Initialize black piece actor
					ABasePiece* Actor = GameMode->GField->BasePieceMap[BlackPieceLocation2d];

					//Add black piece to blackActors array
					BlackActors.Add(Actor);
				}
			}
		}	
			// Check if there is at least one black piece
			if (BlackActors.Num() > 0)
			{	
				// Iterate on BlackActors array
				for (ABasePiece* PossiblePiece : BlackActors)
				{
					// Get iterated black actor location
					FVector ActorLocation = PossiblePiece->GetActorLocation();

					// Try to cast PossiblePice to AKing and check if cast is successful 
					if (AKing* KingActor = Cast<AKing>(PossiblePiece))
					{
						// Calcolate King(PossiblePiece) possible moves
						BKingPossibleMoves(ActorLocation);

						// Check if there is at least one possible move for the king
						if(PossibleBKingMoves.Num() > 0)
						{
							// Add King(PossiblePiece) to an array of movable black pieces
							BlackMovableActors.Add(PossiblePiece);
						}
					}

					else if (APawnChess* PawnActor = Cast<APawnChess>(PossiblePiece))
					{
						BPawnPossibleMoves(ActorLocation);

						if (PossibleBPawnMoves.Num() > 0)
						{
							BlackMovableActors.Add(PossiblePiece);
						}
					}

					else if (AQueen* QueenActor = Cast<AQueen>(PossiblePiece))
					{
						BQueenPossibleMoves(ActorLocation);

						if (PossibleBQueenMoves.Num() > 0)
						{
							BlackMovableActors.Add(PossiblePiece);
						}
					}
					else if (ABishop* BishopActor = Cast<ABishop>(PossiblePiece))
					{
						BBishopPossibleMoves(ActorLocation);

						if (PossibleBBishopMoves.Num() > 0)
						{
							BlackMovableActors.Add(PossiblePiece);
						}
					}
					else if (AKnight* KnightActor = Cast<AKnight>(PossiblePiece))
					{
						BKnightPossibleMoves(ActorLocation);

						if (PossibleBKnightMoves.Num() > 0)
						{
							BlackMovableActors.Add(PossiblePiece);
						}

					}
					else if (ARook* RookActor = Cast<ARook>(PossiblePiece))
					{
						BRookPossibleMoves(ActorLocation);

						if (PossibleBRookMoves.Num() > 0)
						{
							BlackMovableActors.Add(PossiblePiece);
						}
					}

				}	
			}

			// Check if there is at least one movable black piece
			if (BlackMovableActors.Num() > 0)
			{
				// Initialize Random index to choose randomly one movable black piece
				int32 RandIdx = FMath::RandRange(0, BlackMovableActors.Num() - 1);

				// Random movable black piece
				RandomSelectedActor = BlackMovableActors[RandIdx];

				// Get actor location
				FVector RandomActorLocation = RandomSelectedActor->GetActorLocation();

				// Try to cast PossiblePice to AKing and check if cast is successful 
				if (AKing* KingActor = Cast<AKing>(RandomSelectedActor))
				{
					// Calcolate King(RandomSelectedActor) possible moves
					BKingPossibleMoves(RandomActorLocation);

					// Check if there is at least one king possible move (just for safety)
					if (PossibleBKingMoves.Num() > 0)
					{	
						// Initialize Random index to choose randomly one king move
						int32 RandIdx1 = FMath::RandRange(0, PossibleBKingMoves.Num()-1);

						// Get move 2d position
						FVector2D RandomPosition= PossibleBKingMoves[RandIdx1];

						// 3d move position
						FVector RandomPosition3d;
						RandomPosition3d.X = RandomPosition.X;
						RandomPosition3d.Y = RandomPosition.Y;

						// All chess pieces are spawned at z=10
						RandomPosition3d.Z = 10.00;

						//Normalize
						FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);

						// Check if BasePieceMap contains a piece in the move position. If yes it's a kill move.
						if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
						{
							// Destroy the white actor in the move position
							GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();

							// Remove from basepiecemap the white actor in the move position
							GameMode->GField->BasePieceMap.Remove(NormalizedPosition);
						}

						// Move black piece into his legal move position 
						MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
					}
				}
				else if (APawnChess* PawnActor = Cast<APawnChess>(RandomSelectedActor))
				{
					BPawnPossibleMoves(RandomActorLocation);

					if (PossibleBPawnMoves.Num() > 0)
					{
						int32 RandIdx1 = FMath::RandRange(0, PossibleBPawnMoves.Num() - 1);
						FVector2D RandomPosition = PossibleBPawnMoves[RandIdx1];
						FVector RandomPosition3d;
						RandomPosition3d.X = RandomPosition.X;
						RandomPosition3d.Y = RandomPosition.Y;
						RandomPosition3d.Z = 10.00;

						FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
						if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
						{
							GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
							GameMode->GField->BasePieceMap.Remove(NormalizedPosition);
						}
						MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
					}
				}
				else if (AQueen* QueenActor = Cast<AQueen>(RandomSelectedActor))
				{
					BQueenPossibleMoves(RandomActorLocation);

					if (PossibleBQueenMoves.Num() > 0)
					{
						int32 RandIdx1 = FMath::RandRange(0, PossibleBQueenMoves.Num() - 1);
						FVector2D RandomPosition = PossibleBQueenMoves[RandIdx1];
						FVector RandomPosition3d;
						RandomPosition3d.X = RandomPosition.X;
						RandomPosition3d.Y = RandomPosition.Y;
						RandomPosition3d.Z = 10.00;
						FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
						if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
						{
							GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
							GameMode->GField->BasePieceMap.Remove(NormalizedPosition);
						}
						MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
					}
				}
				else if (ABishop* BishopActor = Cast<ABishop>(RandomSelectedActor))
				{
					BBishopPossibleMoves(RandomActorLocation);

					if (PossibleBBishopMoves.Num() > 0)
					{
						int32 RandIdx1 = FMath::RandRange(0, PossibleBBishopMoves.Num() - 1);
						FVector2D RandomPosition = PossibleBBishopMoves[RandIdx1];
						FVector RandomPosition3d;
						RandomPosition3d.X = RandomPosition.X;
						RandomPosition3d.Y = RandomPosition.Y;
						RandomPosition3d.Z = 10.00;
						FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
						if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
						{
							GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
							GameMode->GField->BasePieceMap.Remove(NormalizedPosition);
						}
						MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
					}
				}
				else if (AKnight* KnightActor = Cast<AKnight>(RandomSelectedActor))
				{
					BKnightPossibleMoves(RandomActorLocation);

					if (PossibleBKnightMoves.Num() > 0)
					{
						int32 RandIdx1 = FMath::RandRange(0, PossibleBKnightMoves.Num() - 1);
						FVector2D RandomPosition = PossibleBKnightMoves[RandIdx1];
						FVector RandomPosition3d;
						RandomPosition3d.X = RandomPosition.X;
						RandomPosition3d.Y = RandomPosition.Y;
						RandomPosition3d.Z = 10.00;
						FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
						if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
						{
							GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
							GameMode->GField->BasePieceMap.Remove(NormalizedPosition);
						}
						MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
					}
				}
				else if (ARook* RookActor = Cast<ARook>(RandomSelectedActor))
				{
					BRookPossibleMoves(RandomActorLocation);

					if (PossibleBRookMoves.Num() > 0)
					{
						for (int32 i = 0; i < PossibleBRookMoves.Num(); ++i)
						{
							FVector2D RandomPosition = PossibleBRookMoves[i];
							FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
						}
						int32 RandIdx1 = FMath::RandRange(0, PossibleBRookMoves.Num() - 1);
						FVector2D RandomPosition = PossibleBRookMoves[RandIdx1];
						FVector RandomPosition3d;
						RandomPosition3d.X = RandomPosition.X;
						RandomPosition3d.Y = RandomPosition.Y;
						RandomPosition3d.Z = 10.00;
						FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
						if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
						{
							GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
							GameMode->GField->BasePieceMap.Remove(NormalizedPosition);
						}
						MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
					}
				}
			}
	// 1 second timer
	}, 1, false);
}




// Move black piece into new location
void ACHS_RandomPlayer::MoveBaseBlackPiece(ABasePiece*, FVector OldLocation, FVector NewLocation)
{
	// GameMode pointer
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Set black piece into new location
	RandomSelectedActor->SetActorLocation(NewLocation);

	// Old black piece location in 2d
	FVector2D OldBlackActorLocation2D(OldLocation);

	// Get new piece location after the move
	FVector NewActorLocation = RandomSelectedActor->GetActorLocation();

	// New 2d location
	FVector2D NewActorLocation2D(NewActorLocation);

	// Check if actor moved 
	if (NewActorLocation == NewLocation)
	{
		// Set tile located at old piece position as empty and without owner
		GameMode->GField->TileMap[OldBlackActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);

		// Set tile located at new black piece position as occupied and with black owner
		GameMode->GField->TileMap[NewActorLocation2D]->SetTileStatus(ETileOwner::BLACK, ETileStatus::OCCUPIED);
	}
	// Normalize
	OldBlackActorLocation2D.X = OldBlackActorLocation2D.X / 120;
	OldBlackActorLocation2D.Y = OldBlackActorLocation2D.Y / 120;
	NewActorLocation2D.X = NewActorLocation2D.X / 120;
	NewActorLocation2D.Y = NewActorLocation2D.Y / 120;

	// Change piece key from his old location to his new location
	GameMode->GField->BasePieceMap.Remove(OldBlackActorLocation2D);
	GameMode->GField->BasePieceMap.Add(NewActorLocation2D, RandomSelectedActor);

	// Check if the piece is a pawn and if it reached the other side of the chessboard
	if (RandomSelectedActor->IsA(APawnChess::StaticClass()) && NewActorLocation2D.X == 0)
	{
		// Initialize PossiblePromotionPiece string array
		TArray<FString> PossiblePromotionPiece = {
			FString("Queen"),
			FString("Rook"),
			FString("Bishop"),
			FString("Knight"),

		};

		// Initialize random index to choose randomly pawn promotion
		int32 Random = FMath::RandRange(0, PossiblePromotionPiece.Num() - 1);

		// Initialize string
		FString Piece = PossiblePromotionPiece[Random];

		// Call PawnPromotionFunction. ( 2 == BLACK)
		GameMode->GField->PawnPromotion(RandomSelectedActor, 2, Piece);

	}


	// End AI turn 
	GameMode->EndAITurn();
}

// Calculate black knight possible moves
void ACHS_RandomPlayer::BKnightPossibleMoves(FVector KnightLocation)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleBKnightMoves.Empty();

	TArray<FVector2D> KnightPositions = {
		FVector2D(1, 2),
		FVector2D(2, 1),
		FVector2D(-1, 2),
		FVector2D(-2, 1),
		FVector2D(1, -2),
		FVector2D(2, -1),
		FVector2D(-1, -2),
		FVector2D(-2, -1)
	};


	for (const FVector2D& Positions : KnightPositions)
	{
		FVector2D NextTileLocation(KnightLocation.X + (Positions.X * 120.0), KnightLocation.Y + (Positions.Y * 120.0));


		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{
			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();


			if (Status == ETileStatus::EMPTY)
			{
				PossibleBKnightMoves.Add(NextTileLocation);
			}

			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;


					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBKnightMoves.Add(NextTileLocation);
						}
					}
				}
			}
		}
	}
}



void ACHS_RandomPlayer::BKingPossibleMoves(FVector KingLocation)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleBKingMoves.Empty();

	TArray<FVector2D> KingPositions = {
		FVector2D(1, 0),
		FVector2D(1, 1),
		FVector2D(0, 1),
		FVector2D(-1, 1),
		FVector2D(-1, 0),
		FVector2D(-1, -1),
		FVector2D(0, -1),
		FVector2D(1, -1)
	};


	for (const FVector2D& Positions : KingPositions)
	{
		FVector2D NextTileLocation(KingLocation.X + (Positions.X * 120.0), KingLocation.Y + (Positions.Y * 120.0));


		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{
			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();


			if (Status == ETileStatus::EMPTY)
			{
				PossibleBKingMoves.Add(NextTileLocation);
			}

			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;


					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBKingMoves.Add(NextTileLocation);
						}
					}
				}
			}
		}
	}
}

void ACHS_RandomPlayer::BPawnPossibleMoves(FVector PawnLocation)
{

	PossibleBPawnMoves.Empty();

	// Location for the vertical move (one tile front)
	FVector2D Pawn2dLocation0(PawnLocation.X - 120, PawnLocation.Y);

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Check if possible positions exist on game grid
	if (GameMode->GField->TileMap.Contains(Pawn2dLocation0))
	{

		ETileStatus status0 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

		// Check if pawn frontal tile is empty
		if (status0 == ETileStatus::EMPTY)
		{

			PossibleBPawnMoves.Add(Pawn2dLocation0);
		}
	}

	// Location for the first optional move of 2 frontal tile
	FVector2D Pawn2dLocation1(PawnLocation.X - 240, PawnLocation.Y);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation1))
	{

		ETileStatus status1 = GameMode->GField->TileMap[Pawn2dLocation1]->GetTileStatus();

		ETileStatus status2 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

		// Check if the pawn is at starting position and if the 2 tiles located in row in front of the pawn are empty 
		if (PawnLocation.X == 720.0 && status1 == ETileStatus::EMPTY && status2 == ETileStatus::EMPTY)
		{

			PossibleBPawnMoves.Add(Pawn2dLocation1);
		}
	}

	// Location for the kill move
	FVector2D Pawn2dLocation2(PawnLocation.X - 120, PawnLocation.Y - 120);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation2))
	{

		ETileOwner Owner0 = GameMode->GField->TileMap[Pawn2dLocation2]->GetOwner();

		// Check if the diagonal tile is owned by white piece. If not it isn't a legal move for the pawn.
		if (Owner0 == ETileOwner::WHITE) {

			FVector2D Pawn2dLocation2Normalized;
			Pawn2dLocation2Normalized.X = Pawn2dLocation2.X / 120;
			Pawn2dLocation2Normalized.Y = Pawn2dLocation2.Y / 120;

			// Check for safety
			if (GameMode->GField->BasePieceMap.Contains(Pawn2dLocation2Normalized))
			{
				ABasePiece* IsKing = GameMode->GField->BasePieceMap[Pawn2dLocation2Normalized];

				// Check if the the white actor is king. if not legal move
				if (!(IsKing->IsA(AKing::StaticClass())))
				{
					PossibleBPawnMoves.Add(Pawn2dLocation2);
					//TODO potrei inserire un counter per lo scacco nell'else di questi if
				}
			}
		}
	}

	// Location for the kill move
	FVector2D Pawn2dLocation3(PawnLocation.X - 120, PawnLocation.Y + 120);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation3))
	{

		ETileOwner Owner1 = GameMode->GField->TileMap[Pawn2dLocation3]->GetOwner();

		// Check if the diagonal tile is owned by white piece. If not it isn't a legal move for the pawn.
		if (Owner1 == ETileOwner::WHITE) {

			FVector2D Pawn2dLocation2Normalized;
			Pawn2dLocation2Normalized.X = Pawn2dLocation3.X / 120;
			Pawn2dLocation2Normalized.Y = Pawn2dLocation3.Y / 120;

			// Check for safety
			if (GameMode->GField->BasePieceMap.Contains(Pawn2dLocation2Normalized))
			{
				ABasePiece* IsKing = GameMode->GField->BasePieceMap[Pawn2dLocation2Normalized];

				// Check if the the white actor is king. if not legal move
				if (!(IsKing->IsA(AKing::StaticClass())))
				{
					PossibleBPawnMoves.Add(Pawn2dLocation3);
					//TODO potrei inserire un counter per lo scacco nell'else di questi if
				}
			}
		}
	}
}

void ACHS_RandomPlayer::BRookPossibleMoves(FVector RookLocation)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleBRookMoves.Empty();

	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(RookLocation.X, RookLocation.Y + (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBRookMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
				}
				break;
			}
			else {
				break;
			}
		}
	}


	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(RookLocation.X, RookLocation.Y - (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBRookMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
				}
				break;
			}
			else {
				break;
			}
		}
	}


	for (int32 i = 1; i <= 7; ++i) {

		FVector2D NextTileLocation(RookLocation.X + (i * 120.0), RookLocation.Y);

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBRookMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
				}
				break;
			}
			else {
				// Esc from cycle if a Tile is occupied by white piece
				break;
			}
		}
	}


	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(RookLocation.X - (i * 120.0), RookLocation.Y);

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBRookMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
				}
				break;
			}
			else {
				break;
			}
		}
	}
}

void ACHS_RandomPlayer::BBishopPossibleMoves(FVector BishopLocation)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());


	PossibleBBishopMoves.Empty();


	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(BishopLocation.X + (i * 120.0f), BishopLocation.Y + (i * 120.0f));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBBishopMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
				}
				break;
			}
			else {
				break;
			}
		}
	}


	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(BishopLocation.X + (i * 120.0), BishopLocation.Y - (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBBishopMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
				}
				break;
			}
			else {
				break;
			}
		}
	}


	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(BishopLocation.X - (i * 120.0), BishopLocation.Y + (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBBishopMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
				}
				break;
			}
			else {
				break;
			}
		}
	}


	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(BishopLocation.X - (i * 120.0), BishopLocation.Y - (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleBBishopMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
				}
				break;
			}
			else {
				break;
			}
		}
	}
}

void ACHS_RandomPlayer::BQueenPossibleMoves(FVector QueenLocation)
{

	PossibleBQueenMoves.Empty();

	// Calculate rook possible moves using queen position
	BRookPossibleMoves(QueenLocation);

	// Calculate bishop possible moves using queen position
	BBishopPossibleMoves(QueenLocation);

	// Copy at the end of possiblequeenmoves array the due possible move array -> queen has both legal moves
	PossibleBQueenMoves.Append(PossibleBRookMoves);
	PossibleBQueenMoves.Append(PossibleBBishopMoves);
}
