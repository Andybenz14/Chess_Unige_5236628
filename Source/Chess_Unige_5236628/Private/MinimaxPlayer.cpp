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
	IsKillMove = false;
	bool IsMax = true;
	int32 Depth = 2;
	int32 Alfa = std::numeric_limits<int>::min();
	int32 Beta = std::numeric_limits<int>::max();
	FVector2D BestMove;
	ABasePiece* BestActor = nullptr;
	TMap<FVector2D, ABasePiece*>Backup = GameMode->GField->BasePieceMap;
	int32 test=AlfaBetaMinimax(Depth, IsMax, Alfa, Beta, BestMove, BestActor);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Black Score: %d"), test));

	GameMode->GField->BasePieceMap = Backup;
	TileMapReset();
	if (BestActor != nullptr)
	{
		BestMove.X = BestMove.X / 120;
		BestMove.Y = BestMove.Y / 120;
		SetKilledPieceHidden(BestMove);
		FVector NewLocation(BestMove.X * 120, BestMove.Y * 120, 10.00);
		FVector OldLocation = BestActor->GetActorLocation();
		MoveBaseBlackPiece(BestActor, OldLocation, NewLocation);
	}
	else
	{
		GameInstance->SetTurnMessage(TEXT("WHITE WINS"));
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		PlayerController->DisableInput(PlayerController);
		GameInstance->IsGameFinished = true;
	}
}

//Il Problema è che gli array dei cicli cambiano nel mentre
int32 AMinimaxPlayer::AlfaBetaMinimax(int32 Depth, bool IsMax, int32 Alfa, int32 Beta, FVector2D& BestMove, ABasePiece*& BestActor)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	if (Depth == 0 )
	{
		return Evaluate();
	}

	if (IsMax)
	{
		IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
		IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);

		if (GameInstance->IsGameFinished)
		{
			GameInstance->IsGameFinished = false;
			return std::numeric_limits<int>::min();
		}

		V = std::numeric_limits<int>::min();

		TArray<ABasePiece*> ActorsBackup = Actors;
		//Da capire come mai a volte gli actors siano più di 16
		for (ABasePiece* SelectedActor : ActorsBackup)
		{
			FVector ActorLocation = SelectedActor->GetActorLocation();

			// Try to cast PossiblePice to AKing and check if cast is successful 
			if (AKing* KingActor = Cast<AKing>(SelectedActor))
			{
				IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
				IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);
				KingPossibleMoves(ActorLocation, ETileOwner::WHITE);
				TArray<FVector2D> PossibleKingMovesBackup = PossibleKingMoves;

				if (PossibleKingMovesBackup.Num() > 0)
				{
					for (FVector2D Move : PossibleKingMovesBackup)
					{
						FVector StartLocation = SelectedActor->GetActorLocation();
						MoveSimulation(StartLocation, Move, ETileOwner::BLACK, ETileOwner::WHITE, SelectedActor, Depth, Alfa, Beta, BestMove, BestActor);

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

		return V;
	}
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
		//Qui eseguo tutti i nodi possibili
		for (ABasePiece* SelectedActor : ActorsBackup)
		{
			FVector ActorLocation = SelectedActor->GetActorLocation();

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Actor: %s"), *SelectedActor->GetName()));
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

void AMinimaxPlayer::MoveSimulation(FVector SelectedActorLocation, FVector2D SelectedMovePosition, ETileOwner FriendColor, ETileOwner EnemyColor, ABasePiece* SelectedActor, int32 Depth, int32 Alfa, int32 Beta, FVector2D& BestMove, ABasePiece*& BestActor)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
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
	// Set tile located at old piece position as empty and without owner
	// Set tile located at old piece position as empty and without owner
	GameMode->GField->TileMap[SelectedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);

	// Set tile located at new black piece position as occupied and with black owner
	GameMode->GField->TileMap[SelectedMovePosition]->SetTileStatus(FriendColor, ETileStatus::OCCUPIED);

	// Normalize
	FVector2D SelectedActorLocation2DNormalized;
	SelectedActorLocation2DNormalized.X = SelectedActorLocation2D.X / 120;
	SelectedActorLocation2DNormalized.Y = SelectedActorLocation2D.Y / 120;


	// Change piece key from his old location to his new location
	GameMode->GField->BasePieceMap.Remove(SelectedActorLocation2DNormalized);
	GameMode->GField->BasePieceMap.Add(NormalizedPosition, SelectedActor);

	if (FriendColor == ETileOwner::BLACK)
	{
		V = std::max(V, AlfaBetaMinimax(Depth - 1, false, Alfa, Beta, BestMove, BestActor));
	}
	else
	{
		V = std::min(V, AlfaBetaMinimax(Depth - 1, true, Alfa, Beta, BestMove, BestActor));
	}

	GameMode->GField->BasePieceMap=BackupBasePieceMap;
	TileMapReset();
	
}


int32 AMinimaxPlayer::Evaluate()
{
	int BlackScore = 0;
	int WhiteScore = 0;

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	
	for (auto& Pair : GameMode->GField->BasePieceMap)
	{
		ABasePiece* BasePieceActor = Pair.Value;

		if (BasePieceActor->GetPieceColor() == EPieceColor::BLACK)
		{
			if (BasePieceActor->IsA(APawnChess::StaticClass()))
			{
				BlackScore = BlackScore + 1;
			}
			else if (BasePieceActor->IsA(AQueen::StaticClass()))
			{
				BlackScore = BlackScore + 9;
			}
			else if (BasePieceActor->IsA(ABishop::StaticClass()))
			{
				BlackScore = BlackScore + 3;
			}
			else if (BasePieceActor->IsA(AKnight::StaticClass()))
			{
				BlackScore = BlackScore + 3;
			}
			else if (BasePieceActor->IsA(ARook::StaticClass()))
			{
				BlackScore = BlackScore + 5;
			}
		}
		else
		{
			if (BasePieceActor->IsA(APawnChess::StaticClass()))
			{
				WhiteScore = WhiteScore + 1;
			}
			else if (BasePieceActor->IsA(AQueen::StaticClass()))
			{
				WhiteScore = WhiteScore + 9;
			}
			else if (BasePieceActor->IsA(ABishop::StaticClass()))
			{
				WhiteScore = WhiteScore + 3;
			}
			else if (BasePieceActor->IsA(AKnight::StaticClass()))
			{
				WhiteScore = WhiteScore + 3;
			}
			else if (BasePieceActor->IsA(ARook::StaticClass()))
			{
				WhiteScore = WhiteScore + 5;
			}
		}
	}
		
	

	return BlackScore-WhiteScore;
}




void AMinimaxPlayer::SetKilledPieceHidden(FVector2D NormalizedPosition)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
	{
		IsKillMove = true;

		ABasePiece* KilledActor = GameMode->GField->BasePieceMap[NormalizedPosition];
		// Destroy the white actor in the move position
		KilledActor->SetActorHiddenInGame(true);
		KilledActor->SetActorEnableCollision(false);

		// Remove from basepiecemap the white actor in the move position
		GameMode->GField->BasePieceMap.Remove(NormalizedPosition);

		FDestroyedPiece NewDestroyedPiece;
		NewDestroyedPiece.Piece = KilledActor;
		NewDestroyedPiece.Position = NormalizedPosition;
		NewDestroyedPiece.TurnCounter = GameInstance->DestroyedPieceArrayIndexCounter;
		GameInstance->DestroyedPieceArray.Add(NewDestroyedPiece);
	}

}

void AMinimaxPlayer::TileMapReset()
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	for (auto& Pair : GameMode->GField->TileMap)
	{
		ATile* Tile = Pair.Value;
		Tile->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		FVector TileLocation = Tile->GetActorLocation();
		FVector2D TileLocation2D(TileLocation.X / 120, TileLocation.Y / 120);

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
	RegisterMoveConverter(FVector2D(NewActorLocation2D.X + 1, NewActorLocation2D.Y + 1), FVector2D(OldBlackActorLocation2D.X + 1, OldBlackActorLocation2D.Y + 1), SelectedActor, ETileOwner::WHITE);

	// Change piece key from his old location to his new location
	GameMode->GField->BasePieceMap.Remove(OldBlackActorLocation2D);
	GameMode->GField->BasePieceMap.Add(NewActorLocation2D, SelectedActor);

	// Check if the piece is a pawn and if it reached the other side of the chessboard
	if (SelectedActor->IsA(APawnChess::StaticClass()) && NewActorLocation2D.X == 0)
	{

		// Call PawnPromotionFunction. ( 2 == BLACK)
		GameMode->GField->PawnPromotion(SelectedActor, 2, "Queen");

	}
	else
	{
		GameInstance->DestroyedPieceArrayIndexCounter++;
	}

	// End AI turn 
	GameMode->EndAITurn();
}










	
