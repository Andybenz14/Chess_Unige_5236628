// Fill out your copyright notice in the Description page of Project Settings.

#include "CHS_RandomPlayer.h"




// Sets default values
ACHS_RandomPlayer::ACHS_RandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UCHS_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

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
	GameInstance->SetTurnMessage(TEXT("Random AI Player Turn"));

	// Timer
	FTimerHandle TimerHandle;

	// Check if black king is on check or on checkmate and calculate all the illegal moves that white can't do this turn
	IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
	IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);

	// Check if game can continue
	if (GameInstance->IsGameFinished == false)
	{

		// Set Timer
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				// Initialize an array of black pieces
				TArray<ABasePiece*> BlackActors;

				// Empty the array of black pieces
				BlackActors.Empty();

				// Empty the array of black movable pieces
				BlackMovableActors.Empty();

				// Bool reset
				IsKillMove = false;

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
					// Iterate on BlackActors array to calculate all possible moves
					for (ABasePiece* PossiblePiece : BlackActors)
					{
						// Get iterated black actor location
						FVector ActorLocation = PossiblePiece->GetActorLocation();

						// Check if is a king
						if (AKing* KingActor = Cast<AKing>(PossiblePiece))
						{
							// Calcolate King(PossiblePiece) possible moves
							KingPossibleMoves(ActorLocation, ETileOwner::WHITE);

							// Check if there is at least one possible move for the king
							if (PossibleKingMoves.Num() > 0)
							{
								// Add King(PossiblePiece) to an array of movable black pieces. 
								// One of this actors will be randomly choose to move
								BlackMovableActors.Add(PossiblePiece);
							}
						}
						else if (APawnChess* PawnActor = Cast<APawnChess>(PossiblePiece))
						{
							PawnPossibleMoves(ActorLocation, ETileOwner::WHITE);

							if (PossiblePawnMoves.Num() > 0)
							{
								BlackMovableActors.Add(PossiblePiece);
							}
						}
						else if (AQueen* QueenActor = Cast<AQueen>(PossiblePiece))
						{
							QueenPossibleMoves(ActorLocation, ETileOwner::WHITE);

							if (PossibleQueenMoves.Num() > 0)
							{
								BlackMovableActors.Add(PossiblePiece);
							}
						}
						else if (ABishop* BishopActor = Cast<ABishop>(PossiblePiece))
						{
							BishopPossibleMoves(ActorLocation, ETileOwner::WHITE);

							if (PossibleBishopMoves.Num() > 0)
							{
								BlackMovableActors.Add(PossiblePiece);
							}
						}
						else if (AKnight* KnightActor = Cast<AKnight>(PossiblePiece))
						{
							KnightPossibleMoves(ActorLocation, ETileOwner::WHITE);

							if (PossibleKnightMoves.Num() > 0)
							{
								BlackMovableActors.Add(PossiblePiece);
							}
						}
						else if (ARook* RookActor = Cast<ARook>(PossiblePiece))
						{
							RookPossibleMoves(ActorLocation, ETileOwner::WHITE);

							if (PossibleRookMoves.Num() > 0)
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

					// Select randomly a Piece to move
					RandomSelectedActor = BlackMovableActors[RandIdx];

					// Get actor location
					FVector RandomActorLocation = RandomSelectedActor->GetActorLocation();

					// Check if is a king
					if (AKing* KingActor = Cast<AKing>(RandomSelectedActor))
					{
						// Calcolate King possible moves
						KingPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						// Check if there is at least one king possible move (just for safety)
						if (PossibleKingMoves.Num() > 0)
						{
							// Select Random Move
							int32 RandIdx1 = FMath::RandRange(0, PossibleKingMoves.Num() - 1);
							FVector2D RandomPosition = PossibleKingMoves[RandIdx1];
							FVector RandomPosition3d(RandomPosition.X, RandomPosition.Y, 10.00);
							FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);

							// Set killed piece random
							SetKilledPieceHidden(NormalizedPosition);

							// Move piece
							MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
						}
					}
					else if (APawnChess* PawnActor = Cast<APawnChess>(RandomSelectedActor))
					{
						PawnPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossiblePawnMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossiblePawnMoves.Num() - 1);
							FVector2D RandomPosition = PossiblePawnMoves[RandIdx1];
							FVector RandomPosition3d(RandomPosition.X, RandomPosition.Y, 10.00);
							FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);

							SetKilledPieceHidden(NormalizedPosition);
							MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
						}
					}
					else if (AQueen* QueenActor = Cast<AQueen>(RandomSelectedActor))
					{
						QueenPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossibleQueenMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossibleQueenMoves.Num() - 1);
							FVector2D RandomPosition = PossibleQueenMoves[RandIdx1];
							FVector RandomPosition3d(RandomPosition.X, RandomPosition.Y, 10.00);
							FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);

							SetKilledPieceHidden(NormalizedPosition);
							MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
						}
					}
					else if (ABishop* BishopActor = Cast<ABishop>(RandomSelectedActor))
					{
						BishopPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossibleBishopMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossibleBishopMoves.Num() - 1);
							FVector2D RandomPosition = PossibleBishopMoves[RandIdx1];
							FVector RandomPosition3d(RandomPosition.X, RandomPosition.Y, 10.00);
							FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);

							SetKilledPieceHidden(NormalizedPosition);
							MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
						}
					}
					else if (AKnight* KnightActor = Cast<AKnight>(RandomSelectedActor))
					{
						KnightPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossibleKnightMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossibleKnightMoves.Num() - 1);
							FVector2D RandomPosition = PossibleKnightMoves[RandIdx1];
							FVector RandomPosition3d(RandomPosition.X, RandomPosition.Y, 10.00);
							FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);

							SetKilledPieceHidden(NormalizedPosition);
							MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
						}
					}
					else if (ARook* RookActor = Cast<ARook>(RandomSelectedActor))
					{
						RookPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossibleRookMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossibleRookMoves.Num() - 1);
							FVector2D RandomPosition = PossibleRookMoves[RandIdx1];
							FVector RandomPosition3d(RandomPosition.X, RandomPosition.Y, 10.00);
							FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);

							SetKilledPieceHidden(NormalizedPosition);
							MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
						}
					}
				}
				// 1 second timer
			}, 0.5, false);
	}
}

void ACHS_RandomPlayer::SetKilledPieceHidden(FVector2D NormalizedPosition)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// If it's a kill move
	if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
	{
		// It's a kill move. Information used by register
		IsKillMove = true;

		ABasePiece* KilledActor = GameMode->GField->BasePieceMap[NormalizedPosition];

		// Set hidden the white actor in the move position
		KilledActor->SetActorHiddenInGame(true);
		KilledActor->SetActorEnableCollision(false);

		// Remove from basepiecemap the white actor in the move position
		GameMode->GField->BasePieceMap.Remove(NormalizedPosition);

		// Create new object for replay
		FDestroyedPiece NewDestroyedPiece;
		NewDestroyedPiece.Piece = KilledActor;
		NewDestroyedPiece.Position = NormalizedPosition;
		NewDestroyedPiece.TurnCounter = GameInstance->DestroyedPieceArrayIndexCounter;
		GameInstance->DestroyedPieceArray.Add(NewDestroyedPiece);
	}

}


// Move black piece into new location
void ACHS_RandomPlayer::MoveBaseBlackPiece(ABasePiece*, FVector OldLocation, FVector NewLocation)
{
	// GameMode pointer
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Set black piece into move location
	RandomSelectedActor->SetActorLocation(NewLocation);

	// Old black piece location in 2d
	FVector2D OldBlackActorLocation2D(OldLocation);

	// Get new piece location after the move
	FVector NewActorLocation = RandomSelectedActor->GetActorLocation();

	// Add new move for replay
	GameInstance->MovesForReplay.Add(NewActorLocation);
	GameInstance->PiecesForReplay.Add(RandomSelectedActor);

	// New 2d location
	FVector2D NewActorLocation2D(NewActorLocation);

	// Check if actor moved 
	if (NewActorLocation == NewLocation)
	{
		// Update tiles status and owners
		GameMode->GField->TileMap[OldBlackActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		GameMode->GField->TileMap[NewActorLocation2D]->SetTileStatus(ETileOwner::BLACK, ETileStatus::OCCUPIED);
	}

	// Normalize
	OldBlackActorLocation2D.X = OldBlackActorLocation2D.X / 120;
	OldBlackActorLocation2D.Y = OldBlackActorLocation2D.Y / 120;
	NewActorLocation2D.X = NewActorLocation2D.X / 120;
	NewActorLocation2D.Y = NewActorLocation2D.Y / 120;

	// Reset register turn index
	if (GameInstance->Moves.Num() == 1)
	{
		RegisterMovesCounter = 0;
	}

	// Send register new move
	RegisterMoveConverter(FVector2D(NewActorLocation2D.X + 1, NewActorLocation2D.Y + 1), FVector2D(OldBlackActorLocation2D.X + 1, OldBlackActorLocation2D.Y + 1), RandomSelectedActor, ETileOwner::WHITE);
	
	// Remove moved piece from map
	GameMode->GField->BasePieceMap.Remove(OldBlackActorLocation2D);
	// Update map with the moved actor in his new location
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
	else
	{
		// Update turn index
		GameInstance->DestroyedPieceArrayIndexCounter++;
	}
	
	// End AI turn 
	GameMode->EndAITurn();
}

