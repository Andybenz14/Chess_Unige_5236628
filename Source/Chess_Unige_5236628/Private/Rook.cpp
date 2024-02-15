// Fill out your copyright notice in the Description page of Project Settings.


#include "Rook.h"
#include "CHS_GameMode.h"

// Sets default values
ARook::ARook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARook::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ARook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARook::RookPossibleMoves(FVector RookLocation, ETileOwner Color)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleRookMoves.Empty();

	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(RookLocation.X, RookLocation.Y + (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleRookMoves.Add(NextTileLocation);
				GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == Color)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleRookMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
						}
						else
						{
							GameMode->GField->Check = true;
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
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
				PossibleRookMoves.Add(NextTileLocation);
				GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == Color)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleRookMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
						}
						else
						{
							GameMode->GField->Check = true;
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
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
				PossibleRookMoves.Add(NextTileLocation);
				GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == Color)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleRookMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
						}
						else
						{
							GameMode->GField->Check = true;
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
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
				PossibleRookMoves.Add(NextTileLocation);
				GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == Color)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							PossibleRookMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
						}
						else
						{
							GameMode->GField->Check = true;
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
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