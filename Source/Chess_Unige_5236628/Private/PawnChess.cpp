// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnChess.h"
#include "CHS_GameMode.h"
#include "BasePiece.h"
#include "GameField.h"

// Sets default values
APawnChess::APawnChess()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APawnChess::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnChess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnChess::PawnPossibleMoves(FVector PawnLocation, ETileOwner Color)
{


	PossiblePawnMoves.Empty();

	FVector2D Pawn2dLocation0(PawnLocation.X + 120, PawnLocation.Y);

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation0))
	{

		ETileStatus status0 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

		if (status0 == ETileStatus::EMPTY)
		{

			PossiblePawnMoves.Add(Pawn2dLocation0);
			GameMode->GField->AllPossibleMoves.Add(Pawn2dLocation0);
		}
	}

	FVector2D Pawn2dLocation1(PawnLocation.X + 240, PawnLocation.Y);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation1))
	{

		ETileStatus status1 = GameMode->GField->TileMap[Pawn2dLocation1]->GetTileStatus();
		ETileStatus status2 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

		if (PawnLocation.X == 120.0 && status1 == ETileStatus::EMPTY && status2 == ETileStatus::EMPTY)
		{

			PossiblePawnMoves.Add(Pawn2dLocation1);
			GameMode->GField->AllPossibleMoves.Add(Pawn2dLocation1);
		}
	}

	FVector2D Pawn2dLocation2(PawnLocation.X + 120, PawnLocation.Y + 120);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation2))
	{

		ETileOwner Owner0 = GameMode->GField->TileMap[Pawn2dLocation2]->GetOwner();

		if (Owner0 == Color) {

			FVector2D Pawn2dLocation2Normalized;
			Pawn2dLocation2Normalized.X = Pawn2dLocation2.X / 120;
			Pawn2dLocation2Normalized.Y = Pawn2dLocation2.Y / 120;

			if (GameMode->GField->BasePieceMap.Contains(Pawn2dLocation2Normalized))
			{
				ABasePiece* IsKing = GameMode->GField->BasePieceMap[Pawn2dLocation2Normalized];
				if (!(IsKing->IsA(AKing::StaticClass())))
				{
					PossiblePawnMoves.Add(Pawn2dLocation2);
					//TODO potrei inserire un counter per lo scacco nell'else di questi if
					GameMode->GField->AllPossibleMoves.Add(Pawn2dLocation2);
				}
				else
				{
					GameMode->GField->Check = true;
					GameMode->GField->AllPossibleMoves.Add(Pawn2dLocation2);
				}
			}
		}
	}

	FVector2D Pawn2dLocation3(PawnLocation.X + 120, PawnLocation.Y - 120);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation3))
	{

		ETileOwner Owner1 = GameMode->GField->TileMap[Pawn2dLocation3]->GetOwner();

		if (Owner1 == Color) {

			FVector2D Pawn2dLocation3Normalized;
			Pawn2dLocation3Normalized.X = Pawn2dLocation3.X / 120;
			Pawn2dLocation3Normalized.Y = Pawn2dLocation3.Y / 120;

			if (GameMode->GField->BasePieceMap.Contains(Pawn2dLocation3Normalized))
			{
				ABasePiece* IsKing = GameMode->GField->BasePieceMap[Pawn2dLocation3Normalized];
				if (!(IsKing->IsA(AKing::StaticClass())))
				{
					PossiblePawnMoves.Add(Pawn2dLocation3);
					//TODO potrei inserire un counter per lo scacco nell'else di questi if
					GameMode->GField->AllPossibleMoves.Add(Pawn2dLocation3);
				}
				else
				{
					GameMode->GField->Check = true;
					GameMode->GField->AllPossibleMoves.Add(Pawn2dLocation3);
				}
			}
		}
	}

}