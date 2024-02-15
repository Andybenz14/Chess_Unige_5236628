// Fill out your copyright notice in the Description page of Project Settings.


#include "Bishop.h"
#include "CHS_GameMode.h"

// Sets default values
ABishop::ABishop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BasePieceGridPosition = FVector2D(0, 0);

}

// Called when the game starts or when spawned
void ABishop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABishop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABishop::BishopPossibleMoves(FVector BishopLocation, ETileOwner Color)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());


	PossibleBishopMoves.Empty();


	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(BishopLocation.X + (i * 120.0f), BishopLocation.Y + (i * 120.0f));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBishopMoves.Add(NextTileLocation);
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
							PossibleBishopMoves.Add(NextTileLocation);
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

		FVector2D NextTileLocation(BishopLocation.X + (i * 120.0), BishopLocation.Y - (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBishopMoves.Add(NextTileLocation);
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
							PossibleBishopMoves.Add(NextTileLocation);
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
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

		FVector2D NextTileLocation(BishopLocation.X - (i * 120.0), BishopLocation.Y + (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBishopMoves.Add(NextTileLocation);
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
							PossibleBishopMoves.Add(NextTileLocation);
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
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

		FVector2D NextTileLocation(BishopLocation.X - (i * 120.0), BishopLocation.Y - (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBishopMoves.Add(NextTileLocation);
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
							PossibleBishopMoves.Add(NextTileLocation);
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