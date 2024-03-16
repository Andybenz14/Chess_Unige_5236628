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
	int32 Depth = 0;
	int32 Alfa = std::numeric_limits<int>::min();
	int32 Beta = std::numeric_limits<int>::max();
	NodeCounter = 0;
	PiecesMapForMinimax.Empty();
	IsCheckKing(ETileOwner::BLACK, ETileOwner::WHITE);
	IsCheckMate(ETileOwner::BLACK, ETileOwner::WHITE);
	AlfaBetaMinimax(UpdateInGamePiecesMapForMinimaxSimulation(), Depth, Alfa, Beta, IsMax);
	
}

TMap<FVector2D, ABasePiece*> AMinimaxPlayer::UpdateInGamePiecesMapForMinimaxSimulation()
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	for (auto& Pair : GameMode->GField->TileMap)
	{
		ATile* Tile = Pair.Value;
		FVector TileLocation = Tile->GetActorLocation();
		FVector2D TileLocationNormalized(TileLocation.X / 120, TileLocation.Y / 120);

		if (GameMode->GField->BasePieceMap.Contains(TileLocationNormalized))
		{
			ABasePiece* BasePieceActor = GameMode->GField->BasePieceMap[TileLocationNormalized];
			PiecesMapForMinimax.Add(TileLocationNormalized, BasePieceActor);
		}
	}

	return PiecesMapForMinimax;
}

int32 AMinimaxPlayer::AlfaBetaMinimax(const TMap<FVector2D, ABasePiece*>& State, int32 Depth, int32 Alfa, int32 Beta, bool IsMax)
{
	if (Depth == 0 || NodeCounter == Depth)
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
			ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
			// Get actor location
			FVector ActorLocation = SelectedActor->GetActorLocation();
			FVector2D ActorLocation2D(ActorLocation);
			FVector2D SelectedActorLocationNormalized(ActorLocation.X / 120, ActorLocation.Y / 120);

			// Try to cast PossiblePice to AKing and check if cast is successful 
			if (AKing* KingActor = Cast<AKing>(SelectedActor))
			{
				for (FVector2D Move : PossibleKingMoves)
				{
					GameMode->GField->TileMap[ActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
					GameMode->GField->TileMap[Move]->SetTileStatus(ETileOwner::BLACK, ETileStatus::OCCUPIED);

					FVector2D MoveNormalized(Move.X / 120, Move.Y / 120);
					if (GameMode->GField->BasePieceMap.Contains(MoveNormalized))
					{
						GameMode->GField->BasePieceMap.Remove(MoveNormalized);
					}

					GameMode->GField->BasePieceMap.Remove(SelectedActorLocationNormalized);
					GameMode->GField->BasePieceMap.Add(MoveNormalized, SelectedActor);

					V = std::max(V, AlfaBetaMinimax(/*Devo passare la mappa modificata */, Depth - 1, Alfa, Beta, false));
				}
			}
			else if (APawnChess* PawnActor = Cast<APawnChess>(SelectedActor))
			{

				V = std::max(V, AlfaBetaMinimax(/*Devo passare la mappa modificata */, Depth - 1, Alfa, Beta, false));
			}
			else if (AQueen* QueenActor = Cast<AQueen>(SelectedActor))
			{

				V = std::max(V, AlfaBetaMinimax(/*Devo passare la mappa modificata */, Depth - 1, Alfa, Beta, false));
			}
			else if (ABishop* BishopActor = Cast<ABishop>(SelectedActor))
			{

				V = std::max(V, AlfaBetaMinimax(/*Devo passare la mappa modificata */, Depth - 1, Alfa, Beta, false));
			}
			else if (AKnight* KnightActor = Cast<AKnight>(SelectedActor))
			{

				V = std::max(V, AlfaBetaMinimax(/*Devo passare la mappa modificata */, Depth - 1, Alfa, Beta, false));
			}
			else if (ARook* RookActor = Cast<ARook>(SelectedActor))
			{
				V = std::max(V, AlfaBetaMinimax(/*Devo passare la mappa modificata */, Depth - 1, Alfa, Beta, false));
			}

		}

	}


	NodeCounter++;
	return int32();
}


int32 AMinimaxPlayer::Evaluate()
{
	int BlackScore = 0;
	int WhiteScore = 0;

	for (auto& Pair : PiecesMapForMinimax)
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
		
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Black Score: %d, White Score: %d"), BlackScore, WhiteScore));

	return /*da definire*/ 0;
}












	
