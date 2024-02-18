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
	WaitFunction = true;
	IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
	
	if (WaitFunction == false)
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
							KingPossibleMoves(ActorLocation, ETileOwner::WHITE);

							// Check if there is at least one possible move for the king
							if (PossibleKingMoves.Num() > 0)
							{
								// Add King(PossiblePiece) to an array of movable black pieces
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

					// Random movable black piece
					RandomSelectedActor = BlackMovableActors[RandIdx];

					// Get actor location
					FVector RandomActorLocation = RandomSelectedActor->GetActorLocation();

					// Try to cast PossiblePice to AKing and check if cast is successful 
					if (AKing* KingActor = Cast<AKing>(RandomSelectedActor))
					{
						// Calcolate King(RandomSelectedActor) possible moves
						KingPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						// Check if there is at least one king possible move (just for safety)
						if (PossibleKingMoves.Num() > 0)
						{
							// Initialize Random index to choose randomly one king move
							int32 RandIdx1 = FMath::RandRange(0, PossibleKingMoves.Num() - 1);

							// Get move 2d position
							FVector2D RandomPosition = PossibleKingMoves[RandIdx1];

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
						PawnPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossiblePawnMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossiblePawnMoves.Num() - 1);
							FVector2D RandomPosition = PossiblePawnMoves[RandIdx1];
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
						QueenPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossibleQueenMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossibleQueenMoves.Num() - 1);
							FVector2D RandomPosition = PossibleQueenMoves[RandIdx1];
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
						BishopPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossibleBishopMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossibleBishopMoves.Num() - 1);
							FVector2D RandomPosition = PossibleBishopMoves[RandIdx1];
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
						KnightPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossibleKnightMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossibleKnightMoves.Num() - 1);
							FVector2D RandomPosition = PossibleKnightMoves[RandIdx1];
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
						RookPossibleMoves(RandomActorLocation, ETileOwner::WHITE);

						if (PossibleRookMoves.Num() > 0)
						{
							int32 RandIdx1 = FMath::RandRange(0, PossibleRookMoves.Num() - 1);
							FVector2D RandomPosition = PossibleRookMoves[RandIdx1];
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

			/*if ((IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK)) == true) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("White wins"));


			}*/
		

	}
	/*
				for (const FVector2D& Vec : PossiblePawnMoves)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, VecAsString);
				}
				for (const FVector2D& Vec : PossibleKingMoves)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, VecAsString);
				}
				for (const FVector2D& Vec : PossibleBishopMoves)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, VecAsString);
				}

				for (const FVector2D& Vec : PossibleRookMoves)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Blue, VecAsString);
				}
				for (const FVector2D& Vec : PossibleKnightMoves)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Purple, VecAsString);
				}
				for (const FVector2D& Vec : PossibleQueenMoves)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Orange, VecAsString);
				}*/
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
