// Fill out your copyright notice in the Description page of Project Settings.


#include "King.h"
#include "CHS_GameMode.h"

// Sets default values
AKing::AKing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKing::KingPossibleMoves(FVector KingLocation, ETileOwner Color)
{

	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleKingMoves.Empty();

	TArray<FVector2D> KingPositions = {
		FVector2D(1, 0),
		FVector2D(1, 1),
		FVector2D(0, 1),
		FVector2D(-1, 1),
		FVector2D(-1, 0),
		FVector2D(-1, -1),
		FVector2D(0, -1),
		FVector2D(1, -1)
	};


	for (const FVector2D& Positions : KingPositions)
	{
		FVector2D NextTileLocation(KingLocation.X + (Positions.X * 120.0), KingLocation.Y + (Positions.Y * 120.0));


		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{
			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();


			if (Status == ETileStatus::EMPTY)
			{
				PossibleKingMoves.Add(NextTileLocation);
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
							PossibleKingMoves.Add(NextTileLocation);
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
						}
						else
						{
							GameMode->GField->Check = true;
							GameMode->GField->AllPossibleMoves.Add(NextTileLocation);
						}
					}
				}
			}
		}
	}

}
