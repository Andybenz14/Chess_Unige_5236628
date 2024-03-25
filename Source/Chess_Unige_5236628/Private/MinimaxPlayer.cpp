// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimaxPlayer.h"
#include <limits>
#include <algorithm>

// Sets default values
AMinimaxPlayer::AMinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMinimaxPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinimaxPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMinimaxPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinimaxPlayer::OnTurn()
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Reset 
	IsKillMove = false;
	// AI is the max user
	bool IsMax = true;
	// Minimax Depth
	int32 Depth = 2;
	// Set ALFA = -inf
	int32 Alfa = std::numeric_limits<int>::min();
	// Set BETA = +inf
	int32 Beta = std::numeric_limits<int>::max();

	// Inizialize best move and the actor to move
	FVector2D BestMove;
	ABasePiece* BestActor = nullptr;

	// Pieces map backup for safety
	TMap<FVector2D, ABasePiece*>Backup = GameMode->GField->BasePieceMap;

	// Minimax call
	int32 test=AlfaBetaMinimax(Depth, IsMax, Alfa, Beta, BestMove, BestActor);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Black Score: %d"), test));

	// Reset pieces map for safety
	GameMode->GField->BasePieceMap = Backup;
	// Reset tile status for safety
	TileMapReset();

	// If minimax found a move
	if (BestActor != nullptr)
	{
		BestMove.X = BestMove.X / 120;
		BestMove.Y = BestMove.Y / 120;

		// Set white killed piece hidden
		SetKilledPieceHidden(BestMove);
		FVector NewLocation(BestMove.X * 120, BestMove.Y * 120, 10.00);
		FVector OldLocation = BestActor->GetActorLocation();

		// Move black piece into best move position
		MoveBaseBlackPiece(BestActor, OldLocation, NewLocation);
	}
	else
	{
		// White wins if minimax doesn't find a move to do
		GameInstance->SetTurnMessage(TEXT("WHITE WINS"));
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		PlayerController->DisableInput(PlayerController);
		GameInstance->IsGameFinished = true;
	}
}


int32 AMinimaxPlayer::AlfaBetaMinimax(int32 Depth, bool IsMax, int32 Alfa, int32 Beta, FVector2D& BestMove, ABasePiece*& BestActor)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// If depth = 0 calls the evaluation of the node
	if (Depth == 0 )
	{
		return Evaluate();
	}

	// If it's black
	if (IsMax)
	{
		// Calculate all the illegal moves that the black can't do
		IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
		IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);

		// If it finds a check mate on black returns -inf
		if (GameInstance->IsGameFinished)
		{
			GameInstance->IsGameFinished = false;
			return std::numeric_limits<int>::min();
		}

		// Best value = -inf
		V = std::numeric_limits<int>::min();

		// Actors is the array of black in-game actors calculated with IsCheckKing()
		TArray<ABasePiece*> ActorsBackup = Actors;
		
		// Iterate over all black in-game actors
		for (ABasePiece* SelectedActor : ActorsBackup)
		{
			FVector ActorLocation = SelectedActor->GetActorLocation();

			// If is a king 
			if (AKing* KingActor = Cast<AKing>(SelectedActor))
			{
				// Calculate all the illegal moves
				IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
				IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);

				// Calculate king legal moves
				KingPossibleMoves(ActorLocation, ETileOwner::WHITE);

				// Save the legal moves into a backup. Each depth will have his immutable 
				// backup moves array for each piece where can iterate without changes
				TArray<FVector2D> PossibleKingMovesBackup = PossibleKingMoves;

				if (PossibleKingMovesBackup.Num() > 0)
				{
					// Iterate over legal moves
					for (FVector2D Move : PossibleKingMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						// Simulate the selected move
						// This function contains recursive call to AlphaBetaMinimax()
						MoveSimulation(StartLocation, Move, ETileOwner::BLACK, ETileOwner::WHITE, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						// Alfa-Beta pruning
						if (V > Alfa) {
							Alfa = V;

							// Pick best move
							BestMove = Move;
							BestActor = SelectedActor;
						}

						if (V >= Beta)
						{
							return V;
						}
					}
				}
			}
			else if (APawnChess* PawnActor = Cast<APawnChess>(SelectedActor))
			{
				IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
				IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);
				PawnPossibleMoves(ActorLocation, ETileOwner::WHITE);

				TArray<FVector2D> PossiblePawnMovesBackup = PossiblePawnMoves;

				if (PossiblePawnMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossiblePawnMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::BLACK, ETileOwner::WHITE, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						// Find white king location
						FVector2D KingLocation = FindKingLocation();

						// If the selected piece location is not in the tile next to the king.
						// This is to prevent a white king instant kill of the black piece in the next turn
						if (FMath::Abs(KingLocation.X - Move.X) > 120 && FMath::Abs(KingLocation.Y - Move.Y) > 120)
						{
							// Check reset
							Check = false;

							FVector Move3d(Move.X, Move.Y, 10);

							// Calculate pawn moves to see if the pawn makes check on the white king
							PawnPossibleMoves(Move3d, ETileOwner::WHITE);

							// If pawn makes check on white king this is best move
							if (Check)
							{
								// Increment best value 
								V += 1;
							}

						}

						if (V > Alfa) {
							Alfa = V;
							BestMove = Move;
							BestActor = SelectedActor;
						}

						if (V >= Beta)
						{
							return V;
						}
					}
				}
			}
			else if (AQueen* QueenActor = Cast<AQueen>(SelectedActor))
			{
				IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
				IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);
				QueenPossibleMoves(ActorLocation, ETileOwner::WHITE);

				TArray<FVector2D> PossibleQueenMovesBackup = PossibleQueenMoves;

				if (PossibleQueenMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleQueenMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::BLACK, ETileOwner::WHITE, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);
						
						FVector2D KingLocation = FindKingLocation();
						
						if (FMath::Abs(KingLocation.X - Move.X) > 120 && FMath::Abs(KingLocation.Y - Move.Y) > 120)
						{
							Check = false;

							FVector Move3d(Move.X, Move.Y, 10);

							QueenPossibleMoves(Move3d, ETileOwner::WHITE);

							if (Check)
							{
								V += 1;
							}
						}

						if (V > Alfa) {
							Alfa = V;
							BestMove = Move;
							BestActor = SelectedActor;
						}

						if (V >= Beta)
						{
							return V;
						}
					}
				}
			}
			else if (ABishop* BishopActor = Cast<ABishop>(SelectedActor))
			{
				IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
				IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);
				BishopPossibleMoves(ActorLocation, ETileOwner::WHITE);

				TArray<FVector2D> PossibleBishopMovesBackup = PossibleBishopMoves;

				if (PossibleBishopMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleBishopMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::BLACK, ETileOwner::WHITE, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						FVector2D KingLocation = FindKingLocation();

						if (FMath::Abs(KingLocation.X - Move.X) > 120 && FMath::Abs(KingLocation.Y - Move.Y) > 120)
						{

							Check = false;

							FVector Move3d(Move.X, Move.Y, 10);

							BishopPossibleMoves(Move3d, ETileOwner::WHITE);

							if (Check)
							{
								V += 1;
							}
						}

						if (V > Alfa) {
							Alfa = V;
							BestMove = Move;
							BestActor = SelectedActor;
						}

						if (V >= Beta)

						{
							return V;
						}
					}
				}
			}
			else if (AKnight* KnightActor = Cast<AKnight>(SelectedActor))
			{
				IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
				IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);
				KnightPossibleMoves(ActorLocation, ETileOwner::WHITE);
				TArray<FVector2D> PossibleKnightMovesBackup = PossibleKnightMoves;

				if (PossibleKnightMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleKnightMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::BLACK, ETileOwner::WHITE, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						FVector2D KingLocation = FindKingLocation();

						if (FMath::Abs(KingLocation.X - Move.X) > 120 && FMath::Abs(KingLocation.Y - Move.Y) > 120)
						{

							Check = false;

							FVector Move3d(Move.X, Move.Y, 10);

							KnightPossibleMoves(Move3d, ETileOwner::WHITE);

							if (Check)
							{
								V += 1;
							}
						}

						if (V > Alfa) {
							Alfa = V;
							BestMove = Move;
							BestActor = SelectedActor;
						}

						if (V >= Beta)
						{
							return V;
						}
					}
				}
			}
			else if (ARook* RookActor = Cast<ARook>(SelectedActor))
			{
				IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
				IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);
				RookPossibleMoves(ActorLocation, ETileOwner::WHITE);

				TArray<FVector2D> PossibleRookMovesBackup = PossibleRookMoves;

				if (PossibleRookMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleRookMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::BLACK, ETileOwner::WHITE, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						FVector2D KingLocation = FindKingLocation();

						if (FMath::Abs(KingLocation.X - Move.X) > 120 && FMath::Abs(KingLocation.Y - Move.Y) > 120)
						{

							Check = false;

							FVector Move3d(Move.X, Move.Y, 10);

							RookPossibleMoves(Move3d, ETileOwner::WHITE);

							if (Check)
							{
								V += 1;
							}

						}

						if (V > Alfa) {
							Alfa = V;
							BestMove = Move;
							BestActor = SelectedActor;
						}

						if (V >= Beta)
						{
							return V;
						}
					}
				}
			}
		}
		// return best value
		return V;
	}
	// Same for white but minimizing
	else
	{
		IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
		IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);

		if (GameInstance->IsGameFinished)
		{
			GameInstance->IsGameFinished = false;
			return std::numeric_limits<int>::max();
		}

		V = std::numeric_limits<int>::max();

		TArray<ABasePiece*> ActorsBackup = Actors;
		
		for (ABasePiece* SelectedActor : ActorsBackup)
		{
			FVector ActorLocation = SelectedActor->GetActorLocation();

			// Try to cast PossiblePice to AKing and check if cast is successful 
			if (AKing* KingActor = Cast<AKing>(SelectedActor))
			{
				IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
				IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);
				KingPossibleMoves(ActorLocation, ETileOwner::BLACK);

				TArray<FVector2D> PossibleKingMovesBackup = PossibleKingMoves;

				if (PossibleKingMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleKingMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::WHITE, ETileOwner::BLACK, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						if (V < Beta) {
							Beta = V;

						}

						if (V <= Alfa)
						{
							return V;
						}
					}
				}
			}
			else if (APawnChess* PawnActor = Cast<APawnChess>(SelectedActor))
			{
				IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
				IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);
				PawnPossibleMoves(ActorLocation, ETileOwner::BLACK);

				TArray<FVector2D> PossiblePawnMovesBackup = PossiblePawnMoves;

				if (PossiblePawnMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossiblePawnMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::WHITE, ETileOwner::BLACK, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						if (V < Beta) {
							Beta = V;
							
						}

						if (V <= Alfa)
						{

							return V;
						}
					}
				}
			}
			else if (AQueen* QueenActor = Cast<AQueen>(SelectedActor))
			{
				IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
				IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);
				QueenPossibleMoves(ActorLocation, ETileOwner::BLACK);

				TArray<FVector2D> PossibleQueenMovesBackup = PossibleQueenMoves;

				if (PossibleQueenMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleQueenMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::WHITE, ETileOwner::BLACK, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						if (V < Beta) {
							Beta = V;
							
						}
						if (V <= Alfa)
						{
							return V;
						}
					}
				}
			}
			else if (ABishop* BishopActor = Cast<ABishop>(SelectedActor))
			{
				IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
				IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);
				BishopPossibleMoves(ActorLocation, ETileOwner::BLACK);

				TArray<FVector2D> PossibleBishopMovesBackup = PossibleBishopMoves;

				if (PossibleBishopMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleBishopMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::WHITE, ETileOwner::BLACK, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						if (V < Beta) {
							Beta = V;
							
						}
						if (V <= Alfa)
						{
							return V;
						}
					}
				}
			}
			else if (AKnight* KnightActor = Cast<AKnight>(SelectedActor))
			{
				IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
				IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);
				KnightPossibleMoves(ActorLocation, ETileOwner::BLACK);

				TArray<FVector2D> PossibleKnightMovesBackup = PossibleKnightMoves;

				if (PossibleKnightMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleKnightMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::WHITE, ETileOwner::BLACK, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						if (V < Beta) {
							Beta = V;
							
						}
						if (V <= Alfa)
						{
							return V;
						}
					}
				}
			}
			else if (ARook* RookActor = Cast<ARook>(SelectedActor))
			{
				IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
				IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);
				RookPossibleMoves(ActorLocation, ETileOwner::BLACK);

				TArray<FVector2D> PossibleRookMovesBackup = PossibleRookMoves;

				if (PossibleRookMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleRookMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();

						MoveSimulation(StartLocation, Move, ETileOwner::WHITE, ETileOwner::BLACK, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

						if (V < Beta) {
							Beta = V;
							
						}
						if (V <= Alfa)
						{
							return V;
						}
					}
				}
			}
		}
		return V;
	}
	
}

// Simulate move to evaluate the new chessboard state. This function contains new minimax calls
void AMinimaxPlayer::MoveSimulation(FVector SelectedActorLocation, FVector2D SelectedMovePosition, ETileOwner FriendColor, ETileOwner EnemyColor, ABasePiece* SelectedActor, int32 Depth, int32 Alfa, int32 Beta, FVector2D& BestMove, ABasePiece*& BestActor)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Pieces backup
	TMap<FVector2D, ABasePiece*> BackupBasePieceMap = GameMode->GField->BasePieceMap;

	//Normalize
	FVector2D NormalizedPosition(SelectedMovePosition.X / 120, SelectedMovePosition.Y / 120);

	// Check if BasePieceMap contains a piece in the move position. If yes it's a kill move.
	if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
	{
		// Remove from basepiecemap the white actor in the move position
		GameMode->GField->BasePieceMap.Remove(NormalizedPosition);

	}

	FVector2D SelectedActorLocation2D(SelectedActorLocation);
	
	// Update tiles status and owner
	GameMode->GField->TileMap[SelectedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
	GameMode->GField->TileMap[SelectedMovePosition]->SetTileStatus(FriendColor, ETileStatus::OCCUPIED);

	// Normalize
	FVector2D SelectedActorLocation2DNormalized;
	SelectedActorLocation2DNormalized.X = SelectedActorLocation2D.X / 120;
	SelectedActorLocation2DNormalized.Y = SelectedActorLocation2D.Y / 120;


	// Update moved piece in map
	GameMode->GField->BasePieceMap.Remove(SelectedActorLocation2DNormalized);
	GameMode->GField->BasePieceMap.Add(NormalizedPosition, SelectedActor);

	// Recursive call to AlphaBetaMinimax() after the move simulation
	if (FriendColor == ETileOwner::BLACK)
	{
		V = std::max(V, AlfaBetaMinimax(Depth - 1, false, Alfa, Beta, BestMove, BestActor));
	}
	else
	{
		V = std::min(V, AlfaBetaMinimax(Depth - 1, true, Alfa, Beta, BestMove, BestActor));
	}

	// Reset BasePieceMap for safety
	GameMode->GField->BasePieceMap=BackupBasePieceMap;
	// Reset tiles for safety
	TileMapReset();
	
}

// Gives a score to chessboard state
int32 AMinimaxPlayer::Evaluate()
{
	int BlackScore = 0;
	int WhiteScore = 0;

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Gets all actors in-game
	for (auto& Pair : GameMode->GField->BasePieceMap)
	{
		ABasePiece* BasePieceActor = Pair.Value;

		FVector ActorLocation = BasePieceActor->GetActorLocation();

		// All black actors
		if (BasePieceActor->GetPieceColor() == EPieceColor::BLACK)
		{
			if (BasePieceActor->IsA(APawnChess::StaticClass()))
			{
				// In game pawn value
				BlackScore += 10;
				BlackScore += EvaluatePawnPromotion(BasePieceActor, 1);
				BlackScore += EvaluateStartingGameOpening(BasePieceActor, 1);
			}
			else if (BasePieceActor->IsA(AQueen::StaticClass()))
			{
				// In game queen value
				BlackScore += 90;

			}
			else if (BasePieceActor->IsA(ABishop::StaticClass()))
			{
				// In game bishop value
				BlackScore += 30;
				BlackScore += EvaluateStartingGameOpening(BasePieceActor, 1);
			}
			else if (BasePieceActor->IsA(AKnight::StaticClass()))
			{
				// In game knight value
				BlackScore += 30;
				BlackScore += EvaluateStartingGameOpening(BasePieceActor, 1);
			}
			else if (BasePieceActor->IsA(ARook::StaticClass()))
			{
				// In game rook value
				BlackScore += 50;
			}
			
		}
		else
		{
			if (BasePieceActor->IsA(APawnChess::StaticClass()))
			{
				WhiteScore += 10;
				WhiteScore += EvaluatePawnPromotion(BasePieceActor, 0);
				WhiteScore += EvaluateStartingGameOpening(BasePieceActor, 0);
			}
			else if (BasePieceActor->IsA(AQueen::StaticClass()))
			{
				WhiteScore += 90;
			}
			else if (BasePieceActor->IsA(ABishop::StaticClass()))
			{
				WhiteScore += 30;
				WhiteScore += EvaluateStartingGameOpening(BasePieceActor, 0);
			}
			else if (BasePieceActor->IsA(AKnight::StaticClass()))
			{
				WhiteScore += 30;
				WhiteScore += EvaluateStartingGameOpening(BasePieceActor, 0);
			}
			else if (BasePieceActor->IsA(ARook::StaticClass()))
			{
				WhiteScore += 50;
			}
		}
	}
	
	// Return the global chessboard state value
	return BlackScore-WhiteScore;
}

// Gives an extra value to the pawn when is going to promote to a queen
int32 AMinimaxPlayer::EvaluatePawnPromotion(ABasePiece* Actor, int32 Color)
{
	int Score = 0;

	FVector ActorLocation = Actor->GetActorLocation();
	if ((ActorLocation.X == 0) && Color == 1)
	{
		// Queen value is 90. So Queen.Value - Pawn.Value = Extra = 80
		Score = 80;
	}
	else if ((ActorLocation.X == 840) && Color == 0)
	{
		Score = 80;
	}


	return Score;
}

// Gives extra value to openings in the first 15 moves 
int32 AMinimaxPlayer::EvaluateStartingGameOpening(ABasePiece* Actor, int32 Color)
{
	int Score = 0;

	FVector ActorLocation = Actor->GetActorLocation();

	if (GameInstance->DestroyedPieceArrayIndexCounter < 15)
	{
		if ((ActorLocation.X == 240 || ActorLocation.X == 360) && (ActorLocation.Y < 360 || ActorLocation.Y > 600) && Color == 0)
		{
			Score = 2;
		}
		else if ((ActorLocation.X == 600 || ActorLocation.X == 480) && (ActorLocation.Y < 360 || ActorLocation.Y > 600) && Color == 1)
		{
			Score = 2;
		}
	}
	

	return Score;
}

// Finds white king location
FVector2D AMinimaxPlayer::FindKingLocation()
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	FVector KingLocation(0, 0, 0);

	for (auto& Pair : GameMode->GField->BasePieceMap)
	{
		ABasePiece* Piece = Pair.Value;

		if (Piece->IsA(AKing::StaticClass()) && Piece->GetPieceColor() == EPieceColor::WHITE)
		{
			KingLocation = Piece->GetActorLocation();
		}
	}
		
	FVector2d KLocation(KingLocation);

	return KLocation;
}

// Set killed piece hidden
void AMinimaxPlayer::SetKilledPieceHidden(FVector2D NormalizedPosition)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// if is killed
	if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
	{
		IsKillMove = true;

		ABasePiece* KilledActor = GameMode->GField->BasePieceMap[NormalizedPosition];

		// Destroy the white actor in the move position
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

// Analyze which pieces are on board and reset tiles status and owner for safety
void AMinimaxPlayer::TileMapReset()
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Iterate over all tiles
	for (auto& Pair : GameMode->GField->TileMap)
	{
		ATile* Tile = Pair.Value;

		// Set All tiles as empty
		Tile->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		FVector TileLocation = Tile->GetActorLocation();
		FVector2D TileLocation2D(TileLocation.X / 120, TileLocation.Y / 120);

		// If there's a piece above the tile set tile owner as the piece color
		if (GameMode->GField->BasePieceMap.Contains(TileLocation2D))
		{
			EPieceColor Color = GameMode->GField->BasePieceMap[TileLocation2D]->GetPieceColor();

			if (Color == EPieceColor::BLACK)
			{
				Tile->SetTileStatus(ETileOwner::BLACK, ETileStatus::OCCUPIED);
			}
			else
			{
				Tile->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
			}
		}
	}

}


// Move black piece into new location
void AMinimaxPlayer::MoveBaseBlackPiece(ABasePiece* SelectedActor, FVector OldLocation, FVector NewLocation)
{
	// GameMode pointer
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Set black piece into new location
	SelectedActor->SetActorLocation(NewLocation);

	// Old black piece location in 2d
	FVector2D OldBlackActorLocation2D(OldLocation);

	// Get new piece location after the move
	FVector NewActorLocation = SelectedActor->GetActorLocation();

	// Replay data
	GameInstance->MovesForReplay.Add(NewActorLocation);
	GameInstance->PiecesForReplay.Add(SelectedActor);

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

	if (GameInstance->Moves.Num() == 1)
	{
		RegisterMovesCounter = 0;
	}

	// Send to register
	RegisterMoveConverter(FVector2D(NewActorLocation2D.X + 1, NewActorLocation2D.Y + 1), FVector2D(OldBlackActorLocation2D.X + 1, OldBlackActorLocation2D.Y + 1), SelectedActor, ETileOwner::WHITE);

	// Change piece key from his old location to his new location
	GameMode->GField->BasePieceMap.Remove(OldBlackActorLocation2D);
	GameMode->GField->BasePieceMap.Add(NewActorLocation2D, SelectedActor);

	// Check if the piece is a pawn and if it reached the other side of the chessboard
	if (SelectedActor->IsA(APawnChess::StaticClass()) && NewActorLocation2D.X == 0)
	{

		// Call PawnPromotionFunction. ( 2 == BLACK) 
		// Set queen promotion by default
		GameMode->GField->PawnPromotion(SelectedActor, 2, "Queen");

	}
	else
	{
		GameInstance->DestroyedPieceArrayIndexCounter++;
	}

	// End AI turn 
	GameMode->EndAITurn();
}










	
