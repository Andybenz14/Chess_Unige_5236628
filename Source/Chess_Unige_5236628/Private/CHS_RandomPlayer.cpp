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
	}
		if (BlackActors.Num() > 0)
		{
			for (ABasePiece* PossiblePiece : BlackActors)
			{
				FVector ActorLocation = PossiblePiece->GetActorLocation();

				if (AKing* KingActor = Cast<AKing>(PossiblePiece))
				{
					BKingPossibleMoves(ActorLocation);

					if(PossibleBKingMoves.Num() > 0)
					{
						BlackMovableActors.Add(PossiblePiece);
					}
				}

				else if (APawnChess* PawnActor = Cast<APawnChess>(PossiblePiece))
				{
					BPawnPossibleMoves(ActorLocation);

					if (PossibleBPawnMoves.Num() > 0)
					{
						BlackMovableActors.Add(PossiblePiece);
					}
				}

				else if (AQueen* QueenActor = Cast<AQueen>(PossiblePiece))
				{
					BQueenPossibleMoves(ActorLocation);

					if (PossibleBQueenMoves.Num() > 0)
					{
						BlackMovableActors.Add(PossiblePiece);
					}
				}
				else if (ABishop* BishopActor = Cast<ABishop>(PossiblePiece))
				{
					BBishopPossibleMoves(ActorLocation);

					if (PossibleBBishopMoves.Num() > 0)
					{
						BlackMovableActors.Add(PossiblePiece);
					}
				}
				else if (AKnight* KnightActor = Cast<AKnight>(PossiblePiece))
				{
					BKnightPossibleMoves(ActorLocation);

					if (PossibleBKnightMoves.Num() > 0)
					{
						BlackMovableActors.Add(PossiblePiece);
					}

				}
				else if (ARook* RookActor = Cast<ARook>(PossiblePiece))
				{
					BRookPossibleMoves(ActorLocation);

					if (PossibleBRookMoves.Num() > 0)
					{
						BlackMovableActors.Add(PossiblePiece);
					}
				}

			}	
		}

		if (BlackMovableActors.Num() > 0)
		{
			int32 RandIdx = FMath::Rand() % BlackMovableActors.Num();
			RandomSelectedActor = BlackMovableActors[RandIdx];
			FVector RandomActorLocation = RandomSelectedActor->GetActorLocation();

			if (AKing* KingActor = Cast<AKing>(RandomSelectedActor))
			{
				
				BKingPossibleMoves(RandomActorLocation);

				
				if (PossibleBKingMoves.Num() > 0)
				{
					int32 RandIdx1 = FMath::Rand() % PossibleBKingMoves.Num();
					FVector2D RandomPosition= PossibleBKingMoves[RandIdx1];
					FVector RandomPosition3d;
					RandomPosition3d.X = RandomPosition.X;
					RandomPosition3d.Y = RandomPosition.Y;
					RandomPosition3d.Z = 10.00;
					FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
					if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
					{
						GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
					}
					MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
				}
			}
			else if (APawnChess* PawnActor = Cast<APawnChess>(RandomSelectedActor))
			{
				BPawnPossibleMoves(RandomActorLocation);

				if (PossibleBPawnMoves.Num() > 0)
				{
					int32 RandIdx1 = FMath::Rand() % PossibleBPawnMoves.Num();
					FVector2D RandomPosition = PossibleBPawnMoves[RandIdx1];
					FVector RandomPosition3d;
					RandomPosition3d.X = RandomPosition.X;
					RandomPosition3d.Y = RandomPosition.Y;
					RandomPosition3d.Z = 10.00;

					FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
					if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
					{
						GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
					}
					MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
				}
			}
			else if (AQueen* QueenActor = Cast<AQueen>(RandomSelectedActor))
			{
				BQueenPossibleMoves(RandomActorLocation);

				if (PossibleBQueenMoves.Num() > 0)
				{
					int32 RandIdx1 = FMath::Rand() % PossibleBQueenMoves.Num();
					FVector2D RandomPosition = PossibleBQueenMoves[RandIdx1];
					FVector RandomPosition3d;
					RandomPosition3d.X = RandomPosition.X;
					RandomPosition3d.Y = RandomPosition.Y;
					RandomPosition3d.Z = 10.00;
					FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
					if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
					{
						GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
					}
					MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
				}
			}
			else if (ABishop* BishopActor = Cast<ABishop>(RandomSelectedActor))
			{
				BBishopPossibleMoves(RandomActorLocation);

				if (PossibleBBishopMoves.Num() > 0)
				{
					int32 RandIdx1 = FMath::Rand() % PossibleBBishopMoves.Num();
					FVector2D RandomPosition = PossibleBBishopMoves[RandIdx1];
					FVector RandomPosition3d;
					RandomPosition3d.X = RandomPosition.X;
					RandomPosition3d.Y = RandomPosition.Y;
					RandomPosition3d.Z = 10.00;
					FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
					if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
					{
						GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
					}
					MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
				}
			}
			else if (AKnight* KnightActor = Cast<AKnight>(RandomSelectedActor))
			{
				BKnightPossibleMoves(RandomActorLocation);

				if (PossibleBKnightMoves.Num() > 0)
				{
					int32 RandIdx1 = FMath::Rand() % PossibleBKnightMoves.Num();
					FVector2D RandomPosition = PossibleBKnightMoves[RandIdx1];
					FVector RandomPosition3d;
					RandomPosition3d.X = RandomPosition.X;
					RandomPosition3d.Y = RandomPosition.Y;
					RandomPosition3d.Z = 10.00;
					FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
					if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
					{
						GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
					}
					MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
				}
			}
			else if (ARook* RookActor = Cast<ARook>(RandomSelectedActor))
			{
				BRookPossibleMoves(RandomActorLocation);

				if (PossibleBRookMoves.Num() > 0)
				{
					int32 RandIdx1 = FMath::Rand() % PossibleBRookMoves.Num();
					FVector2D RandomPosition = PossibleBRookMoves[RandIdx1];
					FVector RandomPosition3d;
					RandomPosition3d.X = RandomPosition.X;
					RandomPosition3d.Y = RandomPosition.Y;
					RandomPosition3d.Z = 10.00;
					FVector2D NormalizedPosition(RandomPosition.X / 120, RandomPosition.Y / 120);
					if (GameMode->GField->BasePieceMap.Contains(NormalizedPosition))
					{
						GameMode->GField->BasePieceMap[NormalizedPosition]->Destroy();
					}
					MoveBaseBlackPiece(RandomSelectedActor, RandomActorLocation, RandomPosition3d);
				}
			}


		}
}





void ACHS_RandomPlayer::MoveBaseBlackPiece(ABasePiece*, FVector OldLocation, FVector NewLocation)
{

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	RandomSelectedActor->SetActorLocation(NewLocation);
	FVector2D ClickedActorLocation2D(OldLocation);
	FVector NewActorLocation = RandomSelectedActor->GetActorLocation();
	FVector2D NewActorLocation2D(NewActorLocation);

	// Check if actor moved 
	if (NewActorLocation == NewLocation)
	{
		GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		GameMode->GField->TileMap[NewActorLocation2D]->SetTileStatus(ETileOwner::BLACK, ETileStatus::OCCUPIED);
	}

	GameMode->EndAITurn();
}

void ACHS_RandomPlayer::BKnightPossibleMoves(FVector KnightLocation)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleBKnightMoves.Empty();

	TArray<FVector2D> KnightPositions = {
		FVector2D(KnightLocation.X + 120, KnightLocation.Y + 240),
		FVector2D(KnightLocation.X + 120, KnightLocation.Y - 240),
		FVector2D(KnightLocation.X - 120, KnightLocation.Y + 240),
		FVector2D(KnightLocation.X - 120, KnightLocation.Y - 240),
		FVector2D(KnightLocation.X + 240, KnightLocation.Y + 120),
		FVector2D(KnightLocation.X + 240, KnightLocation.Y - 120),
		FVector2D(KnightLocation.X - 240, KnightLocation.Y + 120),
		FVector2D(KnightLocation.X - 240, KnightLocation.Y - 120)
	};

	for (const FVector2D& Position : KnightPositions)
	{
		if (GameMode->GField->TileMap.Contains(Position))
		{
			ATile* NextTile = GameMode->GField->TileMap[Position];
			ETileStatus Status = NextTile->GetTileStatus();
			ETileOwner TOwner = NextTile->GetOwner();

			if (Status == ETileStatus::EMPTY || (Status == ETileStatus::OCCUPIED && TOwner == ETileOwner::WHITE))
			{
				PossibleBKnightMoves.Add(Position);

				
			}
		}
	}
}



void ACHS_RandomPlayer::BKingPossibleMoves(FVector KingLocation)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleBKingMoves.Empty();

	// Movimenti orizzontali
	for (int32 i = -1; i <= 1; ++i)
	{
		for (int32 j = -1; j <= 1; ++j)
		{
		
			FVector2D NextTileLocation(KingLocation.X + (i * 120.0), KingLocation.Y + (j * 120.0));

			if (GameMode->GField->TileMap.Contains(NextTileLocation))
			{
				ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
				ETileStatus Status = NextTile->GetTileStatus();
				ETileOwner TOwner = NextTile->GetOwner();

				if (Status == ETileStatus::EMPTY)
				{
					PossibleBKingMoves.Add(NextTileLocation);
				}
				else if (Status == ETileStatus::OCCUPIED && TOwner == ETileOwner::WHITE)
				{
					PossibleBKingMoves.Add(NextTileLocation);
				}
			}
		}
	}
}

void ACHS_RandomPlayer::BPawnPossibleMoves(FVector PawnLocation)
{

	PossibleBPawnMoves.Empty();

	FVector2D Pawn2dLocation0(PawnLocation.X - 120, PawnLocation.Y);

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation0))
	{

		ETileStatus status0 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();

		if (status0 == ETileStatus::EMPTY)
		{

			PossibleBPawnMoves.Add(FVector2D(PawnLocation.X - 120.0, PawnLocation.Y));
		}
	}

	FVector2D Pawn2dLocation3(PawnLocation.X - 240, PawnLocation.Y);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation3))
	{

		ETileStatus status1 = GameMode->GField->TileMap[Pawn2dLocation3]->GetTileStatus();

		if (PawnLocation.X == 720.0 && status1 == ETileStatus::EMPTY)
		{

			PossibleBPawnMoves.Add(FVector2D(PawnLocation.X - 240.0, PawnLocation.Y));
		}
	}

	FVector2D Pawn2dLocation1(PawnLocation.X - 120, PawnLocation.Y - 120);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation1))
	{

		ETileOwner Owner0 = GameMode->GField->TileMap[Pawn2dLocation1]->GetOwner();

		if (Owner0 == ETileOwner::WHITE) {

			PossibleBPawnMoves.Add(FVector2D(PawnLocation.X - 120.0, PawnLocation.Y - 120.0));
		}
	}

	FVector2D Pawn2dLocation2(PawnLocation.X - 120, PawnLocation.Y + 120);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation2))
	{

		ETileOwner Owner1 = GameMode->GField->TileMap[Pawn2dLocation2]->GetOwner();

		if (Owner1 == ETileOwner::WHITE)
		{

			PossibleBPawnMoves.Add(FVector2D(PawnLocation.X - 120.0, PawnLocation.Y + 120.0));
		}
	}
}

void ACHS_RandomPlayer::BRookPossibleMoves(FVector RookLocation)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleBRookMoves.Empty();

	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(RookLocation.X, RookLocation.Y + (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					PossibleBRookMoves.Add(NextTileLocation);
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
				PossibleBRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					PossibleBRookMoves.Add(NextTileLocation);
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
				PossibleBRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					PossibleBRookMoves.Add(NextTileLocation);
				}
				// Esc from cycle if a Tile is occupied by black piece
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
				PossibleBRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					PossibleBRookMoves.Add(NextTileLocation);
				}
				break;
			}
			else {
				break;
			}
		}
	}
}

void ACHS_RandomPlayer::BBishopPossibleMoves(FVector BishopLocation)
{
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	PossibleBBishopMoves.Empty();


	for (int32 i = 1; i <= 7; ++i)
	{

		FVector2D NextTileLocation(BishopLocation.X + (i * 120.0), BishopLocation.Y + (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation))
		{

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				PossibleBBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					PossibleBBishopMoves.Add(NextTileLocation);
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
				PossibleBBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{

					PossibleBBishopMoves.Add(NextTileLocation);
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
				PossibleBBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					PossibleBBishopMoves.Add(NextTileLocation);
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
				PossibleBBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == ETileOwner::WHITE)
				{
					PossibleBBishopMoves.Add(NextTileLocation);
				}
				break;
			}
			else {
				break;
			}
		}
	}
}

void ACHS_RandomPlayer::BQueenPossibleMoves(FVector QueenLocation)
{

	PossibleBQueenMoves.Empty();
	BRookPossibleMoves(QueenLocation);
	BBishopPossibleMoves(QueenLocation);
	PossibleBQueenMoves.Append(PossibleBRookMoves);
	PossibleBQueenMoves.Append(PossibleBBishopMoves);
}
