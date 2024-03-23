// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericPlayer.h"

// Sets default values
AGenericPlayer::AGenericPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UCHS_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called when the game starts or when spawned
void AGenericPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenericPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGenericPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AGenericPlayer::KnightPossibleMoves(FVector KnightLocation, ETileOwner EnemyColor)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleKnightMoves.Empty();
	PossibleKnightMovesForCheck.Empty();

	

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
				if (!(IllegalKnightMoveDueToCheck.Contains(NextTileLocation)))
				{
					PossibleKnightMoves.Add(NextTileLocation);
					PossibleKnightMovesForCheck.Add(NextTileLocation);
				}
			}

			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == EnemyColor)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;


					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							if (!(IllegalKnightMoveDueToCheck.Contains(NextTileLocation)))
							{
								PossibleKnightMoves.Add(NextTileLocation);
								PossibleKnightMovesForCheck.Add(NextTileLocation);
							}
						}
						else
						{
							if (!(IllegalKnightMoveDueToCheck.Contains(NextTileLocation)))
							{
								PossibleKnightMovesForCheck.Add(NextTileLocation);
							}
							Check = true;
						}
					}
				}
			}
		}
	}

}


void AGenericPlayer::KingPossibleMoves(FVector KingLocation, ETileOwner EnemyColor)
{

	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleKingMoves.Empty();
	PossibleKingMovesForCheck.Empty();

	

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
				if (!(IllegalKingMoveDueToCheck.Contains(NextTileLocation)))
				{
					PossibleKingMoves.Add(NextTileLocation);
					PossibleKingMovesForCheck.Add(NextTileLocation);
				}
			}

			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == EnemyColor)
				{
					FVector2D NextTileLocation2dNormalized;
					NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
					NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;


					if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
					{
						ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
						if (!(IsKing->IsA(AKing::StaticClass())))
						{
							if (!(IllegalKingMoveDueToCheck.Contains(NextTileLocation)))
							{
								PossibleKingMoves.Add(NextTileLocation);
								PossibleKingMovesForCheck.Add(NextTileLocation);
							}
						}
						else
						{
							Check = true;
							

							if (!(IllegalKingMoveDueToCheck.Contains(NextTileLocation)))
							{
								PossibleKingMovesForCheck.Add(NextTileLocation);
							}
						}
					}
				}
			}
		}
	}
}

void AGenericPlayer::PawnPossibleMoves(FVector PawnLocation, ETileOwner EnemyColor)
{


	PossiblePawnMoves.Empty();
	PossiblePawnMovesForCheck.Empty();

	

	if (EnemyColor == ETileOwner::BLACK)
	{

		FVector2D Pawn2dLocation0(PawnLocation.X + 120, PawnLocation.Y);

		ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

		if (GameMode->GField->TileMap.Contains(Pawn2dLocation0))
		{

			ETileStatus status0 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

			if (status0 == ETileStatus::EMPTY)
			{
				if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation0)))
				{
					PossiblePawnMoves.Add(Pawn2dLocation0);
					PossiblePawnMovesForCheck.Add(Pawn2dLocation0);
				}
			}
		}

		FVector2D Pawn2dLocation1(PawnLocation.X + 240, PawnLocation.Y);

		if (GameMode->GField->TileMap.Contains(Pawn2dLocation1))
		{

			ETileStatus status1 = GameMode->GField->TileMap[Pawn2dLocation1]->GetTileStatus();
			ETileStatus status2 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

			if (PawnLocation.X == 120.0 && status1 == ETileStatus::EMPTY && status2 == ETileStatus::EMPTY)
			{
				if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation1)))
				{
					PossiblePawnMoves.Add(Pawn2dLocation1);
					PossiblePawnMovesForCheck.Add(Pawn2dLocation1);
				}
			}
		}

		FVector2D Pawn2dLocation2(PawnLocation.X + 120, PawnLocation.Y + 120);

		if (GameMode->GField->TileMap.Contains(Pawn2dLocation2))
		{

			ETileOwner Owner0 = GameMode->GField->TileMap[Pawn2dLocation2]->GetOwner();

			if (Owner0 == EnemyColor) {

				FVector2D Pawn2dLocation2Normalized;
				Pawn2dLocation2Normalized.X = Pawn2dLocation2.X / 120;
				Pawn2dLocation2Normalized.Y = Pawn2dLocation2.Y / 120;

				if (GameMode->GField->BasePieceMap.Contains(Pawn2dLocation2Normalized))
				{
					ABasePiece* IsKing = GameMode->GField->BasePieceMap[Pawn2dLocation2Normalized];
					if (!(IsKing->IsA(AKing::StaticClass())))
					{
						if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation2)))
						{
							PossiblePawnMoves.Add(Pawn2dLocation2);
							PossiblePawnMovesForCheck.Add(Pawn2dLocation2);
						}
						//TODO potrei inserire un counter per lo scacco nell'else di questi if
					}
					else
					{
						if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation2)))
						{
							PossiblePawnMovesForCheck.Add(Pawn2dLocation2);
						}
						Check = true;
				
					}
				}
			}
		}

		FVector2D Pawn2dLocation3(PawnLocation.X + 120, PawnLocation.Y - 120);

		if (GameMode->GField->TileMap.Contains(Pawn2dLocation3))
		{

			ETileOwner Owner1 = GameMode->GField->TileMap[Pawn2dLocation3]->GetOwner();

			if (Owner1 == EnemyColor) {

				FVector2D Pawn2dLocation3Normalized;
				Pawn2dLocation3Normalized.X = Pawn2dLocation3.X / 120;
				Pawn2dLocation3Normalized.Y = Pawn2dLocation3.Y / 120;

				if (GameMode->GField->BasePieceMap.Contains(Pawn2dLocation3Normalized))
				{
					ABasePiece* IsKing = GameMode->GField->BasePieceMap[Pawn2dLocation3Normalized];
					if (!(IsKing->IsA(AKing::StaticClass())))
					{
						if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation3)))
						{
							PossiblePawnMoves.Add(Pawn2dLocation3);
							PossiblePawnMovesForCheck.Add(Pawn2dLocation3);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
					}
					else
					{
						if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation3)))
						{
							PossiblePawnMovesForCheck.Add(Pawn2dLocation3);
							//TODO potrei inserire un counter per lo scacco nell'else di questi if
						}
						Check = true;
				
					}
				}
			}
		}
		
	}
	else
	{
		// Location for the vertical move (one tile front)
		FVector2D Pawn2dLocation0(PawnLocation.X - 120, PawnLocation.Y);

		ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

		// Check if possible positions exist on game grid
		if (GameMode->GField->TileMap.Contains(Pawn2dLocation0))
		{

			ETileStatus status0 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

			// Check if pawn frontal tile is empty
			if (status0 == ETileStatus::EMPTY)
			{
				if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation0)))
				{
					PossiblePawnMoves.Add(Pawn2dLocation0);
					PossiblePawnMovesForCheck.Add(Pawn2dLocation0);
				}
			}
		}

		// Location for the first optional move of 2 frontal tile
		FVector2D Pawn2dLocation1(PawnLocation.X - 240, PawnLocation.Y);

		if (GameMode->GField->TileMap.Contains(Pawn2dLocation1))
		{

			ETileStatus status1 = GameMode->GField->TileMap[Pawn2dLocation1]->GetTileStatus();

			ETileStatus status2 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

			// Check if the pawn is at starting position and if the 2 tiles located in row in front of the pawn are empty 
			if (PawnLocation.X == 720.0 && status1 == ETileStatus::EMPTY && status2 == ETileStatus::EMPTY)
			{
				if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation1)))
				{
					PossiblePawnMoves.Add(Pawn2dLocation1);
					PossiblePawnMovesForCheck.Add(Pawn2dLocation1);
				}
			}
		}

		// Location for the kill move
		FVector2D Pawn2dLocation2(PawnLocation.X - 120, PawnLocation.Y - 120);

		if (GameMode->GField->TileMap.Contains(Pawn2dLocation2))
		{

			ETileOwner Owner0 = GameMode->GField->TileMap[Pawn2dLocation2]->GetOwner();

			// Check if the diagonal tile is owned by white piece. If not it isn't a legal move for the pawn.
			if (Owner0 == ETileOwner::WHITE) {

				FVector2D Pawn2dLocation2Normalized;
				Pawn2dLocation2Normalized.X = Pawn2dLocation2.X / 120;
				Pawn2dLocation2Normalized.Y = Pawn2dLocation2.Y / 120;

				// Check for safety
				if (GameMode->GField->BasePieceMap.Contains(Pawn2dLocation2Normalized))
				{
					ABasePiece* IsKing = GameMode->GField->BasePieceMap[Pawn2dLocation2Normalized];

					// Check if the the white actor is king. if not legal move
					if (!(IsKing->IsA(AKing::StaticClass())))
					{
						if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation2)))
						{
							PossiblePawnMoves.Add(Pawn2dLocation2);
							PossiblePawnMovesForCheck.Add(Pawn2dLocation2);
						}
						//TODO potrei inserire un counter per lo scacco nell'else di questi if
					}
					else
					{
						if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation2)))
						{
							PossiblePawnMovesForCheck.Add(Pawn2dLocation2);
						}
						Check = true;
						

					}
				}
			}
		}

		// Location for the kill move
		FVector2D Pawn2dLocation3(PawnLocation.X - 120, PawnLocation.Y + 120);

		if (GameMode->GField->TileMap.Contains(Pawn2dLocation3))
		{

			ETileOwner Owner1 = GameMode->GField->TileMap[Pawn2dLocation3]->GetOwner();

			// Check if the diagonal tile is owned by white piece. If not it isn't a legal move for the pawn.
			if (Owner1 == ETileOwner::WHITE) {

				FVector2D Pawn2dLocation2Normalized;
				Pawn2dLocation2Normalized.X = Pawn2dLocation3.X / 120;
				Pawn2dLocation2Normalized.Y = Pawn2dLocation3.Y / 120;

				// Check for safety
				if (GameMode->GField->BasePieceMap.Contains(Pawn2dLocation2Normalized))
				{
					ABasePiece* IsKing = GameMode->GField->BasePieceMap[Pawn2dLocation2Normalized];

					// Check if the the white actor is king. if not legal move
					if (!(IsKing->IsA(AKing::StaticClass())))
					{
						if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation3)))
						{
							PossiblePawnMoves.Add(Pawn2dLocation3);
							PossiblePawnMovesForCheck.Add(Pawn2dLocation3);
						}
						//TODO potrei inserire un counter per lo scacco nell'else di questi if
					}
					else
					{
						if (!(IllegalPawnMoveDueToCheck.Contains(Pawn2dLocation3)))
						{
							PossiblePawnMovesForCheck.Add(Pawn2dLocation3);
						}
						Check = true;
					
					}
				}
			}
		}
	}

}

void AGenericPlayer::RookPossibleMoves(FVector RookLocation, ETileOwner EnemyColor)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleRookMoves.Empty();
	PossibleRookMovesForCheck.Empty();



	CalculateRookMoves(RookLocation, FVector2D(0, 1), EnemyColor);
	CalculateRookMoves(RookLocation, FVector2D(0, -1), EnemyColor);
	CalculateRookMoves(RookLocation, FVector2D(1, 0), EnemyColor);
	CalculateRookMoves(RookLocation, FVector2D(-1, 0), EnemyColor);
}

void AGenericPlayer::CalculateRookMoves(FVector RookLocation, FVector2D Direction, ETileOwner EnemyColor)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	FVector2D NextTileLocation(RookLocation.X + (Direction.X * 120.0), RookLocation.Y + (Direction.Y * 120.0));

	if (GameMode->GField->TileMap.Contains(NextTileLocation))
	{
		ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
		ETileStatus Status = NextTile->GetTileStatus();

		if (Status == ETileStatus::EMPTY)
		{
			FVector Location;
			Location.X = NextTileLocation.X;
			Location.Y = NextTileLocation.Y;
			Location.Z = 10;

			if (!(IllegalRookMoveDueToCheck.Contains(NextTileLocation)))
			{
				PossibleRookMoves.Add(NextTileLocation);
				PossibleRookMovesForCheck.Add(NextTileLocation);
			}

			CalculateRookMoves(Location, Direction, EnemyColor);
			
		}
		else if (Status == ETileStatus::OCCUPIED && NextTile->GetOwner() == EnemyColor)
		{
			FVector2D NextTileLocation2dNormalized;
			NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
			NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

			if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
			{
				ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];

				if (!(IsKing->IsA(AKing::StaticClass())))
				{
					
					if (!(IllegalRookMoveDueToCheck.Contains(NextTileLocation)))
					{
						PossibleRookMoves.Add(NextTileLocation);
						PossibleRookMovesForCheck.Add(NextTileLocation);
					}
			
				}
				else
				{
					if (!(IllegalRookMoveDueToCheck.Contains(NextTileLocation)))
					{
						PossibleRookMovesForCheck.Add(NextTileLocation);
					}
					Check = true;
					
				}
			}
		}
	}
}

void AGenericPlayer::BishopPossibleMoves(FVector BishopLocation, ETileOwner EnemyColor)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleBishopMoves.Empty();
	PossibleBishopMovesForCheck.Empty();



	CalculateBishopMoves(BishopLocation, FVector2D(1, 1), EnemyColor);
	CalculateBishopMoves(BishopLocation, FVector2D(1, -1), EnemyColor);
	CalculateBishopMoves(BishopLocation, FVector2D(-1, 1), EnemyColor);
	CalculateBishopMoves(BishopLocation, FVector2D(-1, -1), EnemyColor);
}

void AGenericPlayer::CalculateBishopMoves(FVector BishopLocation, FVector2D Direction, ETileOwner EnemyColor)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	FVector2D NextTileLocation(BishopLocation.X + (Direction.X * 120), BishopLocation.Y + (Direction.Y * 120));

	if (GameMode->GField->TileMap.Contains(NextTileLocation))
	{
		ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
		ETileStatus Status = NextTile->GetTileStatus();

		if (Status == ETileStatus::EMPTY)
		{
			FVector Location;
			Location.X = NextTileLocation.X;
			Location.Y = NextTileLocation.Y;
			Location.Z = 10;

			if (!(IllegalBishopMoveDueToCheck.Contains(NextTileLocation)))
			{
				PossibleBishopMoves.Add(NextTileLocation);
				PossibleBishopMovesForCheck.Add(NextTileLocation);
			}

			CalculateBishopMoves(Location, Direction, EnemyColor); 
		}
		else if (Status == ETileStatus::OCCUPIED && NextTile->GetOwner() == EnemyColor)
		{
			FVector2D NextTileLocation2dNormalized;
			NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
			NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

			if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
			{
				ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
				if (!(IsKing->IsA(AKing::StaticClass())))
				{
					if (!(IllegalBishopMoveDueToCheck.Contains(NextTileLocation)))
					{
						PossibleBishopMoves.Add(NextTileLocation);
						PossibleBishopMovesForCheck.Add(NextTileLocation);
					}
				}
				else
				{
					if (!(IllegalBishopMoveDueToCheck.Contains(NextTileLocation)))
					{
						PossibleBishopMovesForCheck.Add(NextTileLocation);
					}
					Check = true;
			
				}
			}
		}
	}
}

void AGenericPlayer::QueenPossibleMoves(FVector QueenLocation, ETileOwner EnemyColor)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleQueenMoves.Empty();
	PossibleQueenMovesForCheck.Empty();


	
	CalculateQueenMoves(QueenLocation, FVector2D(0, 1), EnemyColor); 
	CalculateQueenMoves(QueenLocation, FVector2D(0, -1), EnemyColor); 
	CalculateQueenMoves(QueenLocation, FVector2D(1, 0), EnemyColor); 
	CalculateQueenMoves(QueenLocation, FVector2D(-1, 0), EnemyColor); 
	CalculateQueenMoves(QueenLocation, FVector2D(1, 1), EnemyColor); 
	CalculateQueenMoves(QueenLocation, FVector2D(-1, 1), EnemyColor); 
	CalculateQueenMoves(QueenLocation, FVector2D(1, -1), EnemyColor); 
	CalculateQueenMoves(QueenLocation, FVector2D(-1, -1), EnemyColor); 
}


void AGenericPlayer::CalculateQueenMoves(FVector QueenLocation, FVector2D Direction, ETileOwner EnemyColor)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	FVector2D NextTileLocation(QueenLocation.X + (Direction.X * 120), QueenLocation.Y + (Direction.Y * 120));

	if (GameMode->GField->TileMap.Contains(NextTileLocation))
	{
		ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
		ETileStatus Status = NextTile->GetTileStatus();

		if (Status == ETileStatus::EMPTY)
		{
			FVector Location;
			Location.X = NextTileLocation.X;
			Location.Y = NextTileLocation.Y;
			Location.Z = 10;

			if (!(IllegalQueenMoveDueToCheck.Contains(NextTileLocation)))
			{
				PossibleQueenMoves.Add(NextTileLocation);
				PossibleQueenMovesForCheck.Add(NextTileLocation);
			}
			CalculateQueenMoves(Location, Direction, EnemyColor);
		}
		else if (Status == ETileStatus::OCCUPIED && NextTile->GetOwner() == EnemyColor)
		{
			FVector2D NextTileLocation2dNormalized;
			NextTileLocation2dNormalized.X = NextTileLocation.X / 120;
			NextTileLocation2dNormalized.Y = NextTileLocation.Y / 120;

			if (GameMode->GField->BasePieceMap.Contains(NextTileLocation2dNormalized))
			{
				ABasePiece* IsKing = GameMode->GField->BasePieceMap[NextTileLocation2dNormalized];
				if (!(IsKing->IsA(AKing::StaticClass())))
				{
					if (!(IllegalQueenMoveDueToCheck.Contains(NextTileLocation)))
					{
						PossibleQueenMoves.Add(NextTileLocation);
						PossibleQueenMovesForCheck.Add(NextTileLocation);
					}
				}
				else
				{
					if (!(IllegalQueenMoveDueToCheck.Contains(NextTileLocation)))
					{
						PossibleQueenMovesForCheck.Add(NextTileLocation);
					}
					Check = true;

					
			
				}
			}
		}
	}
}


void AGenericPlayer::IsCheckKing(ETileOwner FriendColor, ETileOwner EnemyColor)
{
	
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	IllegalKingMoveDueToCheck.Empty();
	IllegalQueenMoveDueToCheck.Empty();
	IllegalPawnMoveDueToCheck.Empty();
	IllegalRookMoveDueToCheck.Empty();
	IllegalBishopMoveDueToCheck.Empty();
	IllegalKnightMoveDueToCheck.Empty();

	// Set to false because could be set to true with precedent calls of possible moves functions
	Check = false;

	SimulatePossibleMoves(FriendColor, EnemyColor);
	
	
}

void AGenericPlayer::SimulatePossibleMoves(ETileOwner FriendColor, ETileOwner EnemyColor)
{			
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	TMap<FVector2D, ABasePiece*> BackupBasePieceMap = GameMode->GField->BasePieceMap;
	//TODO Probaibilmente dovrò fare il reset degli stati delle tile ad ogni iterazione
	
	CalculatePossibleMoves(FriendColor, EnemyColor);
	
	
	TArray<ABasePiece*> ActorsCopy = Actors;
	

	for (ABasePiece* SelectedActor : ActorsCopy)
	{

			// Get actor location
			FVector SelectedActorLocation = SelectedActor->GetActorLocation();
		


			// Try to cast PossiblePice to AKing and check if cast is successful 
			if (AKing* KingActor = Cast<AKing>(SelectedActor))
			{

				KingPossibleMoves(SelectedActorLocation, EnemyColor);
				TArray<FVector2D> PossibleKingMovesForCheckCopy = PossibleKingMovesForCheck;
				Check = false;

				
					for (FVector2D SelectedMovePosition : PossibleKingMovesForCheckCopy)
					{
						MoveSimulation(SelectedActorLocation, SelectedMovePosition, FriendColor, EnemyColor, SelectedActor);
						GameMode->GField->BasePieceMap = BackupBasePieceMap;
					}
				
			}
			else if (APawnChess* PawnActor = Cast<APawnChess>(SelectedActor))
			{
				PawnPossibleMoves(SelectedActorLocation, EnemyColor);
				TArray<FVector2D> PossiblePawnMovesForCheckCopy = PossiblePawnMovesForCheck;
				Check = false;
				for (FVector2D SelectedMovePosition : PossiblePawnMovesForCheckCopy)
				{
					MoveSimulation(SelectedActorLocation, SelectedMovePosition, FriendColor, EnemyColor, SelectedActor);
					GameMode->GField->BasePieceMap = BackupBasePieceMap;
				}
				
			}
			else if (AQueen* QueenActor = Cast<AQueen>(SelectedActor))
			{
				QueenPossibleMoves(SelectedActorLocation, EnemyColor);
				TArray<FVector2D> PossibleQueenMovesForCheckCopy = PossibleQueenMovesForCheck;
				Check = false;
			
					for (FVector2D SelectedMovePosition : PossibleQueenMovesForCheckCopy)
					{
						MoveSimulation(SelectedActorLocation, SelectedMovePosition, FriendColor, EnemyColor, SelectedActor);
						GameMode->GField->BasePieceMap = BackupBasePieceMap;
					}
				
			}
			else if (ABishop* BishopActor = Cast<ABishop>(SelectedActor))
			{
				BishopPossibleMoves(SelectedActorLocation, EnemyColor);
				TArray<FVector2D> PossibleBishopMovesForCheckCopy = PossibleBishopMovesForCheck;
				Check = false;
				
				for (FVector2D SelectedMovePosition : PossibleBishopMovesForCheckCopy)
				{
					MoveSimulation(SelectedActorLocation, SelectedMovePosition, FriendColor, EnemyColor, SelectedActor);
					GameMode->GField->BasePieceMap = BackupBasePieceMap;
				}
			}
			else if (AKnight* KnightActor = Cast<AKnight>(SelectedActor))
			{
				KnightPossibleMoves(SelectedActorLocation, EnemyColor);
				TArray<FVector2D> PossibleKnightMovesForCheckCopy = PossibleKnightMovesForCheck;
				Check = false;
				
				for (FVector2D SelectedMovePosition : PossibleKnightMovesForCheckCopy)
				{
					MoveSimulation(SelectedActorLocation, SelectedMovePosition, FriendColor, EnemyColor, SelectedActor);
					GameMode->GField->BasePieceMap = BackupBasePieceMap;
				}
			}
			else if (ARook* RookActor = Cast<ARook>(SelectedActor))
			{
				RookPossibleMoves(SelectedActorLocation, EnemyColor);
				TArray<FVector2D> PossibleRookMovesForCheckCopy = PossibleRookMovesForCheck;
				Check = false;
			
				for (FVector2D SelectedMovePosition : PossibleRookMovesForCheckCopy)
				{ 
						MoveSimulation(SelectedActorLocation, SelectedMovePosition, FriendColor, EnemyColor, SelectedActor);				
						GameMode->GField->BasePieceMap = BackupBasePieceMap;
					
				}
			}
		
	}
	
}


void AGenericPlayer::CalculatePossibleMoves(ETileOwner FriendColor, ETileOwner EnemyColor)
{

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	PawnIsInGame = 0;
	QueenIsInGame = 0;
	BishopIsInGame = 0;
	KnightIsInGame = 0;
	RookIsInGame = 0;
	NoPawnMovesLeftIndex = 0;
	NoQueenMovesLeftIndex = 0;
	NoBishopMovesLeftIndex = 0;
	NoKnightMovesLeftIndex = 0;
	NoRookMovesLeftIndex = 0;

	Actors.Empty();

	// Iterate on TileArray to find tiles owned by black pieces
	for (const auto& Tile : GameMode->GField->TileArray)
	{
		// Tile owner must be BLACK

		if (Tile->GetOwner() == FriendColor)
		{
			// Get tile location
			FVector ActorLocation = Tile->GetActorLocation();

			// 2D black piece location (same as the tile position)
			FVector2D PieceLocation2d(ActorLocation);

			// Normalize
			PieceLocation2d.X = PieceLocation2d.X / 120;
			PieceLocation2d.Y = PieceLocation2d.Y / 120;

			// Check if BasePieceMap contains the black piece by his 2d location
			if (GameMode->GField->BasePieceMap.Contains(PieceLocation2d))
			{
				// Initialize black piece actor
				ABasePiece* Actor = GameMode->GField->BasePieceMap[PieceLocation2d];

				//Add black piece to blackActors array
				Actors.Add(Actor);
			}
		}
	}
	// Check if there is at least one black piece
	if (Actors.Num() > 0)
	{
		// Iterate on BlackActors array
		for (ABasePiece* PossiblePiece : Actors)
		{
			// Get iterated black actor location
			FVector ActorLocation = PossiblePiece->GetActorLocation();

			// Try to cast PossiblePice to AKing and check if cast is successful 
			if (AKing* KingActor = Cast<AKing>(PossiblePiece))
			{
				// Calcolate King(PossiblePiece) possible moves
				KingPossibleMoves(ActorLocation, EnemyColor);
			}

			else if (APawnChess* PawnActor = Cast<APawnChess>(PossiblePiece))
			{
				PawnPossibleMoves(ActorLocation, EnemyColor);
				PawnIsInGame++;
				if (PossiblePawnMoves.Num() == 0) 
				{
					NoPawnMovesLeftIndex++;
				}
			}

			else if (AQueen* QueenActor = Cast<AQueen>(PossiblePiece))
			{
				QueenPossibleMoves(ActorLocation, EnemyColor);
				QueenIsInGame++;
				if (PossibleQueenMoves.Num() == 0)
				{
					NoQueenMovesLeftIndex++;
				}
			}
			else if (ABishop* BishopActor = Cast<ABishop>(PossiblePiece))
			{
				BishopPossibleMoves(ActorLocation, EnemyColor);
				BishopIsInGame++;
				if (PossibleBishopMoves.Num() == 0)
				{
					NoBishopMovesLeftIndex++;
				}
			}
			else if (AKnight* KnightActor = Cast<AKnight>(PossiblePiece))
			{
				KnightPossibleMoves(ActorLocation, EnemyColor);
				KnightIsInGame++;
				if (PossibleKnightMoves.Num() == 0)
				{
					NoKnightMovesLeftIndex++;
				}

			}
			else if (ARook* RookActor = Cast<ARook>(PossiblePiece))
			{
				RookPossibleMoves(ActorLocation, EnemyColor);
				RookIsInGame++;
				if (PossibleRookMoves.Num() == 0)
				{
					NoRookMovesLeftIndex++;
				}
			}
		}
	}

}

void AGenericPlayer::MoveSimulation(FVector SelectedActorLocation, FVector2D SelectedMovePosition, ETileOwner FriendColor, ETileOwner EnemyColor, ABasePiece* SelectedActor)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
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
	ETileStatus Status0 = GameMode->GField->TileMap[SelectedActorLocation2D]->GetTileStatus();
	ETileOwner Owner0 = GameMode->GField->TileMap[SelectedActorLocation2D]->GetOwner();
	GameMode->GField->TileMap[SelectedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);

	// Set tile located at new black piece position as occupied and with black owner
	ETileStatus Status1 = GameMode->GField->TileMap[SelectedMovePosition]->GetTileStatus();
	ETileOwner Owner1 = GameMode->GField->TileMap[SelectedMovePosition]->GetOwner();
	GameMode->GField->TileMap[SelectedMovePosition]->SetTileStatus(FriendColor, ETileStatus::OCCUPIED);

	// Normalize
	FVector2D SelectedActorLocation2DNormalized;
	SelectedActorLocation2DNormalized.X = SelectedActorLocation2D.X / 120;
	SelectedActorLocation2DNormalized.Y = SelectedActorLocation2D.Y / 120;


	// Change piece key from his old location to his new location
	GameMode->GField->BasePieceMap.Remove(SelectedActorLocation2DNormalized);
	GameMode->GField->BasePieceMap.Add(NormalizedPosition, SelectedActor);

	CalculatePossibleMoves(EnemyColor, FriendColor);

	if (SelectedActor->IsA(AKnight::StaticClass()))
	{
		if (Check == true) {
			IllegalKnightMoveDueToCheck.Add(SelectedMovePosition);

		}
	}
	else if (SelectedActor->IsA(AKing::StaticClass()))
	{
		if (Check == true) {
			IllegalKingMoveDueToCheck.Add(SelectedMovePosition);
		}
	}
	else if (SelectedActor->IsA(APawnChess::StaticClass()))
	{
		if (Check == true) {
			IllegalPawnMoveDueToCheck.Add(SelectedMovePosition);
		}
	}
	else if (SelectedActor->IsA(ARook::StaticClass()))
	{
		if (Check == true) {
			IllegalRookMoveDueToCheck.Add(SelectedMovePosition);
		}
	}
	else if (SelectedActor->IsA(ABishop::StaticClass()))
	{
		if (Check == true) {
			IllegalBishopMoveDueToCheck.Add(SelectedMovePosition);
		}
	}
	else if (SelectedActor->IsA(AQueen::StaticClass()))
	{
		if (Check == true) {
			IllegalQueenMoveDueToCheck.Add(SelectedMovePosition);
		}
	}

	Check = false;
	GameMode->GField->TileMap[SelectedActorLocation2D]->SetTileStatus(Owner0, Status0);
	GameMode->GField->TileMap[SelectedMovePosition]->SetTileStatus(Owner1, Status1);


}

void AGenericPlayer::IsCheckMate(ETileOwner FriendColor, ETileOwner EnemyColor)
{
	CalculatePossibleMoves(FriendColor, EnemyColor);
	if (
		(KnightIsInGame == NoKnightMovesLeftIndex) &&
		(PossibleKingMoves.Num() == 0 ) && 
		(RookIsInGame == NoRookMovesLeftIndex) &&
		(BishopIsInGame == NoBishopMovesLeftIndex) &&
		(PawnIsInGame == NoPawnMovesLeftIndex) &&
		(QueenIsInGame == NoQueenMovesLeftIndex)
	   )
	{
		if(EnemyColor == ETileOwner::WHITE)
		{
			GameInstance->SetTurnMessage(TEXT("WHITE WINS"));
		}
		else
		{
			GameInstance->SetTurnMessage(TEXT("BLACK WINS"));
		}
		
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		PlayerController->DisableInput(PlayerController);
		GameInstance->IsGameFinished = true;
		IllegalKingMoveDueToCheck.Empty();
		IllegalQueenMoveDueToCheck.Empty();
		IllegalPawnMoveDueToCheck.Empty();
		IllegalRookMoveDueToCheck.Empty();
		IllegalBishopMoveDueToCheck.Empty();
		IllegalKnightMoveDueToCheck.Empty();
	}
	


}

void AGenericPlayer::RegisterMoveConverter(FVector2D MovePosition, FVector2D OldPosition, ABasePiece* BasePieceActor, ETileOwner EnemyColor)
{
	RegisterMovesCounter++;

	FString YChar = NumberToCharConverter(MovePosition);
	int XPosition = FMath::RoundToInt(MovePosition.X);
	FString XChar = FString::Printf(TEXT("%d"), XPosition);
	FString CounterString = FString::Printf(TEXT("%d"), RegisterMovesCounter);
	CalculateCheckForRegister(MovePosition, EnemyColor, BasePieceActor);
	
	if (BasePieceActor->IsA(AKnight::StaticClass()))
	{
		if (IsKillMove == false) 
		{
			Message = CounterString + TEXT(": N") + YChar + XChar;
		}
		else
		{
			Message = CounterString + TEXT(": Nx") + YChar + XChar;
		}
	}
	else if (BasePieceActor->IsA(AKing::StaticClass()))
	{
		if (IsKillMove == false)
		{
			Message = CounterString + TEXT(": K") + YChar + XChar;
		}
		else
		{
			Message = CounterString + TEXT(": Kx") + YChar + XChar;
		}
	}
	else if (BasePieceActor->IsA(APawnChess::StaticClass()))
	{
		if (IsKillMove == false)
		{
			Message = CounterString + TEXT(": ") + YChar + XChar;
		}
		else
		{
			Message = CounterString + TEXT(": ") + NumberToCharConverter(OldPosition) + TEXT("x") + YChar + XChar;
		}
	} 
	else if (BasePieceActor->IsA(ARook::StaticClass()))
	{
		if (IsKillMove == false)
		{
			Message = CounterString + TEXT(": R") + YChar + XChar;
		}
		else
		{
			Message = CounterString + TEXT(": Rx") + YChar + XChar;
		}
	}
	else if (BasePieceActor->IsA(ABishop::StaticClass()))
	{
		if (IsKillMove == false)
		{
			Message = CounterString + TEXT(": B") + YChar + XChar;
		}
		else
		{
			Message = CounterString + TEXT(": Bx") + YChar + XChar;
		}
	}
	else if (BasePieceActor->IsA(AQueen::StaticClass()))
	{
		if (IsKillMove == false)
		{
			Message = CounterString + TEXT(": Q") + YChar + XChar;	
		}
		else
		{
			Message = CounterString + TEXT(": Qx") + YChar + XChar;
		}
	}

	if (Check == true) {

		Message = Message + TEXT("+");		
	}

	if (EnemyColor == ETileOwner::WHITE)
	{
		Message = TEXT("(Black) ") + Message;
	}
	else
	{
		Message = TEXT("(White) ") + Message;
	}

	GameInstance->SetRegisterMove(Message);

}

FString AGenericPlayer::NumberToCharConverter(FVector2D MovePosition)
{
	FString PieceLetter;

	if (MovePosition.Y == 1) {
		PieceLetter = "a";
	}
	else if (MovePosition.Y == 2) {
		PieceLetter = "b";
	}
	else if (MovePosition.Y == 3) {
		PieceLetter = "c";
	}
	else if (MovePosition.Y == 4) {
	    PieceLetter = "d";
	}
	else if (MovePosition.Y == 5) {
		PieceLetter = "e";
	}
	else if (MovePosition.Y == 6) {
		PieceLetter = "f";
	}
	else if (MovePosition.Y == 7) {
		PieceLetter = "g";
	}
	else if (MovePosition.Y == 8) {
		PieceLetter = "h";
	}
	
	return PieceLetter;
}

void AGenericPlayer::CalculateCheckForRegister(FVector2D MovePosition, ETileOwner EnemyColor, ABasePiece* PossiblePiece)
{
	FVector ActorLocation((MovePosition.X - 1) * 120, (MovePosition.Y - 1) * 120, 10);
	// Try to cast PossiblePice to AKing and check if cast is successful 
	if (AKing* KingActor = Cast<AKing>(PossiblePiece))
	{
		// Calcolate King(PossiblePiece) possible moves
		KingPossibleMoves(ActorLocation, EnemyColor);

	}

	else if (APawnChess* PawnActor = Cast<APawnChess>(PossiblePiece))
	{
		PawnPossibleMoves(ActorLocation, EnemyColor);
		
	}

	else if (AQueen* QueenActor = Cast<AQueen>(PossiblePiece))
	{
		QueenPossibleMoves(ActorLocation, EnemyColor);
		
	}
	else if (ABishop* BishopActor = Cast<ABishop>(PossiblePiece))
	{
		BishopPossibleMoves(ActorLocation, EnemyColor);
	
	}
	else if (AKnight* KnightActor = Cast<AKnight>(PossiblePiece))
	{
		KnightPossibleMoves(ActorLocation, EnemyColor);
	

	}
	else if (ARook* RookActor = Cast<ARook>(PossiblePiece))
	{
		RookPossibleMoves(ActorLocation, EnemyColor);
	
	}

}



