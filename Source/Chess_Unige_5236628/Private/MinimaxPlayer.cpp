// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimaxPlayer.h"

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
	int gg = 10;

}

int32 AMinimaxPlayer::Evaluate()
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	int BlackScore = 0;
	int WhiteScore = 0;

	for (auto& Pair : GameMode->GField->TileMap)
	{
		ATile* Tile = Pair.Value;
		FVector TileLocation = Tile->GetActorLocation();
		FVector2D TileLocationNormalized(TileLocation.X / 120, TileLocation.Y / 120);

		if (GameMode->GField->BasePieceMap.Contains(TileLocationNormalized))
		{
			ABasePiece* BasePieceActor = GameMode->GField->BasePieceMap[TileLocationNormalized];

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
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Black Score: %d, White Score: %d"), BlackScore, WhiteScore));

	return /*da definire*/ 0;
}
