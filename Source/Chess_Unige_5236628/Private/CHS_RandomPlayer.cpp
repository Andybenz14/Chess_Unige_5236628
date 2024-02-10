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

void ACHS_RandomPlayer::OnTurn()
{
	TArray<ABasePiece*> BlackActors;
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Turn"));
	for (auto& BlackTile : GameMode->GField->GetTileArray())
	{
		if (BlackTile->GetOwner() == ETileOwner::BLACK)
		{
			
			BlackActorLocation = BlackTile->GetActorLocation();
			FVector2D BlackPieceLocation2d(BlackActorLocation);
			BlackPieceLocation2d.X = BlackPieceLocation2d.X / 120;
			BlackPieceLocation2d.Y = BlackPieceLocation2d.Y / 120;
			GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Piece %s"), *BlackPieceLocation2d.ToString()));

			if (GameMode->GField->BasePieceMap.Contains(BlackPieceLocation2d))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Turn second if"));
				ABasePiece* Actor = GameMode->GField->BasePieceMap[BlackPieceLocation2d];
				BlackActors.Add(Actor);
			}
		}

		if (BlackActors.Num() > 0)
		{//TODO ERRORI NELLE POSIZIONI DEGLI ACTOR
			int32 RandIdx = FMath::Rand() % BlackActors.Num();
			ABasePiece* Actor1 = BlackActors[RandIdx];
			FVector ActorLocation = BlackActors[RandIdx]->GetActorLocation();
			GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Piece %s"), *ActorLocation.ToString()));

			/*
			//TODO SALVARE TUTTE LE POSSIBLE MOVES DI TUTTI E POI SCEGLIERNE UNA RANDOMICAMENTE
			if (AKing* KingActor = Cast<AKing>(Actor1))
			{
				GameMode->HPlayer->KingPossibleMoves(ActorLocation);

				if (GameMode->HPlayer->PossibleKingMoves.Num() > 0)
				{
					int32 RandIdx1 = FMath::Rand() % GameMode->HPlayer->PossibleKingMoves.Num();
					FVector2D RandomMoveLocation = GameMode->HPlayer->PossibleKingMoves[RandIdx1];
					FVector RandomMoveLocation3d(RandomMoveLocation.X, RandomMoveLocation.Y, ActorLocation.Z);

					//if (int32 IsValid = GameMode->HPlayer->IsPieceMoveValid(RandomMove, GameMode->HPlayer->PossibleKingMoves) == 1)

					GameMode->HPlayer->MoveBasePiece(Actor1, BlackActorLocation, RandomMoveLocation3d);

				}
			}

			if (APawnChess* PawnActor = Cast<APawnChess>(Actor1))
			{
				GameMode->HPlayer->PawnPossibleMoves(ActorLocation);

				if (GameMode->HPlayer->PossiblePawnMoves.Num() > 0)
				{
					int32 RandIdx1 = FMath::Rand() % GameMode->HPlayer->PossiblePawnMoves.Num();
					FVector2D RandomMoveLocation = GameMode->HPlayer->PossiblePawnMoves[RandIdx1];
					FVector RandomMoveLocation3d(RandomMoveLocation.X, RandomMoveLocation.Y, ActorLocation.Z);

					//if (int32 IsValid = GameMode->HPlayer->IsPieceMoveValid(RandomMove, GameMode->HPlayer->PossibleKingMoves) == 1)

					GameMode->HPlayer->MoveBasePiece(Actor1, BlackActorLocation, RandomMoveLocation3d);

				}
			}

			if (AQueen* QueenActor = Cast<AQueen>(Actor1))
			{

			}
			if (ABishop* BishopActor = Cast<ABishop>(Actor1))
			{

			}
			if (AKnight* KnightActor = Cast<AKnight>(Actor1))
			{

			}
			if (ARook* RookActor = Cast<ARook>(Actor1))
			{

			}*/
		}
	}
}