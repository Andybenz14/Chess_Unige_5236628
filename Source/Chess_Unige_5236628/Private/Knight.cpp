// Fill out your copyright notice in the Description page of Project Settings.


#include "Knight.h"
#include "CHS_GameMode.h"

// Sets default values
AKnight::AKnight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKnight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKnight::KnightPossibleMoves(FVector KnightLocation, ETileOwner Color)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleKnightMoves.Empty();


	TArray<FVector2D> KnightPositions = {
		FVector2D(1, 2),
		FVector2D(2, 1),
		FVector2D(-1, 2),
		FVector2D(-2, 1),
		FVector2D(1, -2),
		FVector2D(2, -1),
		FVector2D(-1, -2),
		FVector2D(-2, -1)
	};


	for (const FVector2D& Positions : KnightPositions)
	{
		FVector2D NextTileLocation(KnightLocation.X + (Positions.X * 120.0), KnightLocation.Y + (Positions.Y * 120.0));


		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{
			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();


			if (Status == ETileStatus::EMPTY)
			{
				PossibleKnightMoves.Add(NextTileLocation);
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
							PossibleKnightMoves.Add(NextTileLocation);
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