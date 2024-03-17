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
	bool IsMax = true;
	int32 Depth = 2;
	int32 Alfa = std::numeric_limits<int>::min();
	int32 Beta = std::numeric_limits<int>::max();
	PieceMapBackupArray.Empty();
	IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
	IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);
	int32 test=AlfaBetaMinimax(Depth, Alfa, Beta, IsMax);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Black Score: %d"), test));

	
}

int32 AMinimaxPlayer::AlfaBetaMinimax(int32 Depth, int32 Alfa, int32 Beta, bool IsMax)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	if (Depth == 3)
	{
		PieceMapBackupArray.SetNum(FMath::Max(PieceMapBackupArray.Num(), 3));
		PieceMapBackupArray[2] = GameMode->GField->BasePieceMap;
	}
	else if (Depth == 2)
	{
		PieceMapBackupArray.SetNum(FMath::Max(PieceMapBackupArray.Num(), 2));
		PieceMapBackupArray[1] = GameMode->GField->BasePieceMap;
	}
	else if (Depth == 1)
	{
		PieceMapBackupArray.SetNum(FMath::Max(PieceMapBackupArray.Num(), 1));
		PieceMapBackupArray[0] = GameMode->GField->BasePieceMap;
	}
	else if (Depth == 0)
	{
		return Evaluate();
	}

	if (IsMax)
	{
		V = std::numeric_limits<int>::min();

		SimulatePossibleMoves(ETileOwner::BLACK, ETileOwner::WHITE);
	
		//Qui eseguo tutti i nodi possibili
		for (ABasePiece* SelectedActor : Actors)
		{
			// Try to cast PossiblePice to AKing and check if cast is successful 
			if (AKing* KingActor = Cast<AKing>(SelectedActor))
			{
				if (PossibleKingMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleKingMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 0);
						V = std::max(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, false));
						if (V >= Beta)
						{
							return V;
						}
						Alfa = std::max(Alfa, V);

					}
					return V;
				}
			}
			else if (APawnChess* PawnActor = Cast<APawnChess>(SelectedActor))
			{
				if (PossiblePawnMoves.Num() > 0)
				{
					for (FVector2D Move : PossiblePawnMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 0);
						V = std::max(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, false));
						if (V >= Beta)
						{
							return V;
						}
						Alfa = std::max(Alfa, V);
					}
					return V;
				}
			}
			else if (AQueen* QueenActor = Cast<AQueen>(SelectedActor))
			{
				if (PossibleQueenMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleQueenMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 0);
						V = std::max(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, false));
						if (V >= Beta)
						{
							return V;
						}
						Alfa = std::max(Alfa, V);

					}
					return V;
				}
			}
			else if (ABishop* BishopActor = Cast<ABishop>(SelectedActor))
			{
				if (PossibleBishopMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleBishopMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 0);
						V = std::max(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, false));
						if (V >= Beta)
						{
							return V;
						}
						Alfa = std::max(Alfa, V);

					}
					return V;
				}
			}
			else if (AKnight* KnightActor = Cast<AKnight>(SelectedActor))
			{
				if (PossibleKnightMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleKnightMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 0);
						V = std::max(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, false));
						if (V >= Beta)
						{
							return V;
						}
						Alfa = std::max(Alfa, V);

					}
					return V;
				}
			}
			else if (ARook* RookActor = Cast<ARook>(SelectedActor))
			{
				if (PossibleRookMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleRookMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 0);
						V = std::max(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, false));
						if (V >= Beta)
						{
							return V;
						}
						Alfa = std::max(Alfa, V);

					}
					return V;
				}
			}

		}

	}
	else
	{

		V = std::numeric_limits<int>::max();

		SimulatePossibleMoves(ETileOwner::WHITE, ETileOwner::BLACK);

		//Qui eseguo tutti i nodi possibili
		for (ABasePiece* SelectedActor : Actors)
		{
			// Try to cast PossiblePice to AKing and check if cast is successful 
			if (AKing* KingActor = Cast<AKing>(SelectedActor))
			{
				if (PossibleKingMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleKingMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 1);
						V = std::min(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, true));
						if (V <= Alfa)
						{
							return V;
						}
						Beta = std::min(Beta, V);

					}
					return V;
				}
			}
			else if (APawnChess* PawnActor = Cast<APawnChess>(SelectedActor))
			{
				if (PossiblePawnMoves.Num() > 0)
				{
					for (FVector2D Move : PossiblePawnMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 1);
						V = std::min(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, true));
						if (V <= Alfa)
						{
							return V;
						}
						Beta = std::min(Beta, V);

					}
					return V;
				}
			}
			else if (AQueen* QueenActor = Cast<AQueen>(SelectedActor))
			{
				if (PossibleQueenMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleQueenMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 1);
						V = std::min(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, true));
						if (V <= Alfa)
						{
							return V;
						}
						Beta = std::min(Beta, V);

					}
					return V;
				}
			}
			else if (ABishop* BishopActor = Cast<ABishop>(SelectedActor))
			{
				if (PossibleBishopMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleBishopMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 1);
						V = std::min(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, true));
						if (V <= Alfa)
						{
							return V;
						}
						Beta = std::min(Beta, V);

					}

					return V;
				}

			}
			else if (AKnight* KnightActor = Cast<AKnight>(SelectedActor))
			{
				if (PossibleKnightMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleKnightMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 1);
						V = std::min(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, true));
						if (V <= Alfa)
						{
							return V;
						}
						Beta = std::min(Beta, V);

					}

					return V;
				}
			}
			else if (ARook* RookActor = Cast<ARook>(SelectedActor))
			{
				if (PossibleRookMoves.Num() > 0)
				{
					for (FVector2D Move : PossibleRookMoves)
					{
						GameMode->GField->BasePieceMap = PieceMapBackupArray[Depth - 1];
						SimulateMove(SelectedActor, Move, 1);
						V = std::min(V, AlfaBetaMinimax(Depth - 1, Alfa, Beta, true));
						if (V <= Alfa)
						{
							return V;
						}
						Beta = std::min(Beta, V);
					}

					return V;
				}
			}

		}


	}
	return V;
}

void AMinimaxPlayer::SimulateMove(ABasePiece* SelectedActor, FVector2D Move, int32 Color)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	// Get actor location
	FVector ActorLocation = SelectedActor->GetActorLocation();
	FVector2D ActorLocation2D(ActorLocation);
	FVector2D SelectedActorLocationNormalized(ActorLocation.X / 120, ActorLocation.Y / 120);

	if (Color == 0)
	{
		GameMode->GField->TileMap[ActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		GameMode->GField->TileMap[Move]->SetTileStatus(ETileOwner::BLACK, ETileStatus::OCCUPIED);
	}
	else
	{
		GameMode->GField->TileMap[ActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		GameMode->GField->TileMap[Move]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
	}

	FVector2D MoveNormalized(Move.X / 120, Move.Y / 120);
	if (GameMode->GField->BasePieceMap.Contains(MoveNormalized))
	{
		GameMode->GField->BasePieceMap.Remove(MoveNormalized);
	}

	GameMode->GField->BasePieceMap.Remove(SelectedActorLocationNormalized);
	GameMode->GField->BasePieceMap.Add(MoveNormalized, SelectedActor);
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
		
	

	return /*da definire*/ 0;
}












	
