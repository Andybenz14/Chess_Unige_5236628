// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_HumanPlayer.h"
#include "CHS_GameMode.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Actor.h"

// Sets default values
ACHS_HumanPlayer::ACHS_HumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//set the camera as RootComponent
	SetRootComponent(Camera);

}

// Called when the game starts or when spawned
void ACHS_HumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACHS_HumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACHS_HumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACHS_HumanPlayer::OnClick()
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);

	SetOriginalTileMaterial();

	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);

	// Hit check and MyTurn check
	if (Hit.bBlockingHit && IsMyTurn)
	{
		// Generic chess piece hit check
		if (ABasePiece* CurrPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			// Get the color of the clicked piece
			EPieceColor Color = CurrPiece->GetPieceColor();

			// Check if the user clicked white piece. User can use only white pieces.
			if (Color == EPieceColor::WHITE) 
			{
				
				ClickCounter = ClickCounter + 1;

				// Get clicked piece xyz location
				ClickedActorLocation = CurrPiece->GetActorLocation();

				// Save the first clicked piece for the future if conditions
				BasePieceActor = CurrPiece;

				if (BasePieceActor->IsA(AKnight::StaticClass())) 
				{
<<<<<<< HEAD
					GameMode->Knight->KnightPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(GameMode->Knight->PossibleKnightMoves);
				}
				else if (BasePieceActor->IsA(AKing::StaticClass())) 
				{
					GameMode->King->KingPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(GameMode->King->PossibleKingMoves);
				}
				else if (BasePieceActor->IsA(APawnChess::StaticClass())) 
				{
					GameMode->PawnChess->PawnPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(GameMode->PawnChess->PossiblePawnMoves);
				}
				else if (BasePieceActor->IsA(ARook::StaticClass())) 
				{
					GameMode->Rook->RookPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(GameMode->Rook->PossibleRookMoves);
				}
				else if (BasePieceActor->IsA(ABishop::StaticClass())) 
				{
					GameMode->Bishop->BishopPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(GameMode->Bishop->PossibleBishopMoves);
				}
				else if (BasePieceActor->IsA(AQueen::StaticClass())) 
				{
					GameMode->Queen->QueenPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(GameMode->Queen->PossibleQueenMoves);
=======
					KnightPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(AKing::StaticClass())) 
				{
					KingPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(APawnChess::StaticClass())) 
				{
					PawnPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(ARook::StaticClass())) 
				{
					RookPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(ABishop::StaticClass())) 
				{
					BishopPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(AQueen::StaticClass())) 
				{
					QueenPossibleMoves(ClickedActorLocation);
>>>>>>> parent of 483c1e1 (Check starting implementatition)
				}
			}
		}
	}

	if (Hit.bBlockingHit && IsMyTurn && ClickCounter != 0)
	{
		

		// Check if the second click is on a tile
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))

		{
			FVector TileLocation = CurrTile->GetActorLocation();
			FVector2d Position(TileLocation);
			ETileStatus Status = CurrTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY)
			{
				if (BasePieceActor->IsA(APawnChess::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->PawnChess->PossiblePawnMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
					}
				}

				else if (BasePieceActor->IsA(AKing::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->King->PossibleKingMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
					
					}
				}

				else if (BasePieceActor->IsA(AQueen::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->Queen->PossibleQueenMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
					
					}
				}

				else if (BasePieceActor->IsA(ABishop::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->Bishop->PossibleBishopMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
					}
				}

				else if (BasePieceActor->IsA(AKnight::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->Knight->PossibleKnightMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
					}
				}

				else if (BasePieceActor->IsA(ARook::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->Rook->PossibleRookMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
					}
				}
			}
		}
		else if (ABasePiece* ClickedPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			EPieceColor Color = ClickedPiece->GetPieceColor();

			if (Color == EPieceColor::BLACK && !(ClickedPiece->IsA(AKing::StaticClass())))
			{

				FVector BlackActorLocation = ClickedPiece->GetActorLocation();
				FVector2D Position (BlackActorLocation);

				if (BasePieceActor->IsA(APawnChess::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->PawnChess->PossiblePawnMoves) == 1)
					{
						ClickedPiece->Destroy();
						GameMode->GField->BasePieceMap.Remove(Position);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
	
				else if (BasePieceActor->IsA(AKing::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->King->PossibleKingMoves) == 1)
					{
						ClickedPiece->Destroy();
						GameMode->GField->BasePieceMap.Remove(Position);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}

				else if (BasePieceActor->IsA(AQueen::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->Queen->PossibleQueenMoves) == 1)
					{
						ClickedPiece->Destroy();
						GameMode->GField->BasePieceMap.Remove(Position);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}

				else if (BasePieceActor->IsA(ABishop::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->Bishop->PossibleBishopMoves) == 1)
					{
						ClickedPiece->Destroy();
						GameMode->GField->BasePieceMap.Remove(Position);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}

				else if (BasePieceActor->IsA(AKnight::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->Knight->PossibleKnightMoves) == 1)
					{
						ClickedPiece->Destroy();
						GameMode->GField->BasePieceMap.Remove(Position);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}

				else if (BasePieceActor->IsA(ARook::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, GameMode->Rook->PossibleRookMoves) == 1)
					{
						ClickedPiece->Destroy();
						GameMode->GField->BasePieceMap.Remove(Position);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
			}
		}
	}
}




void ACHS_HumanPlayer::OnTurn()
{
	IsMyTurn = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your Turn"));
	
}

void ACHS_HumanPlayer::MoveBasePiece(ABasePiece*, FVector OldLocation, FVector NewLocation)
{

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	BasePieceActor->SetActorLocation(NewLocation);
	FVector2D ClickedActorLocation2D(OldLocation);
	FVector NewActorLocation = BasePieceActor->GetActorLocation();
	FVector2D NewActorLocation2D(NewActorLocation);

	// Check if actor moved 
	if (NewActorLocation == NewLocation)
	{
		GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		GameMode->GField->TileMap[NewActorLocation2D]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
	}



	ClickCounter = 0;
	TurnCounter = TurnCounter + 1;
	ClickedActorLocation2D.X = ClickedActorLocation2D.X / 120;
	ClickedActorLocation2D.Y = ClickedActorLocation2D.Y / 120;
	NewActorLocation2D.X = NewActorLocation2D.X / 120;
	NewActorLocation2D.Y = NewActorLocation2D.Y / 120;
	GameMode->GField->BasePieceMap.Remove(ClickedActorLocation2D);
	GameMode->GField->BasePieceMap.Add(NewActorLocation2D, BasePieceActor);
	
	if (BasePieceActor->IsA(APawnChess::StaticClass()) && NewActorLocation2D.X == 7)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

		PlayerController->DisableInput(PlayerController);
		//VA ALLA PAWNPROMOTION 
	}
	else
	{
<<<<<<< HEAD
		GameMode->GField->CheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
=======
>>>>>>> parent of 483c1e1 (Check starting implementatition)
		GameMode->EndHumanTurn();
	}
}

<<<<<<< HEAD
=======
void ACHS_HumanPlayer::KnightPossibleMoves(FVector KnightLocation)
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
			}
		
			else if (Status == ETileStatus::OCCUPIED)
			{
				
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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
						}
					}
				}
			}
		}
	}

	ApplyPossibleMovesMaterials(PossibleKnightMoves);
}


void ACHS_HumanPlayer::KingPossibleMoves(FVector KingLocation) 
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
			}

			else if (Status == ETileStatus::OCCUPIED)
			{

				if (NextTile->GetOwner() == ETileOwner::BLACK)
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
						}
					}
				}
			}
		}
	}

	ApplyPossibleMovesMaterials(PossibleKingMoves);
}

void ACHS_HumanPlayer::PawnPossibleMoves(FVector PawnLocation) 
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
		}
	}

	FVector2D Pawn2dLocation2(PawnLocation.X + 120, PawnLocation.Y + 120);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation2)) 
	{

		ETileOwner Owner0 = GameMode->GField->TileMap[Pawn2dLocation2]->GetOwner();

		if (Owner0 == ETileOwner::BLACK) {

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
				}
			}
		}
	}

	FVector2D Pawn2dLocation3(PawnLocation.X + 120, PawnLocation.Y - 120);

	if (GameMode->GField->TileMap.Contains(Pawn2dLocation3))
	{

		ETileOwner Owner1 = GameMode->GField->TileMap[Pawn2dLocation3]->GetOwner();

		if (Owner1 == ETileOwner::BLACK) {

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
				}
			}
		}
	}

	ApplyPossibleMovesMaterials(PossiblePawnMoves);

}

void ACHS_HumanPlayer::RookPossibleMoves(FVector RookLocation)
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
			}
			else if (Status == ETileStatus::OCCUPIED) 
			{
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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
			}
			else if (Status == ETileStatus::OCCUPIED) 
			{
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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

	ApplyPossibleMovesMaterials(PossibleRookMoves);
}

void ACHS_HumanPlayer::BishopPossibleMoves(FVector BishopLocation)
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
			}
			else if (Status == ETileStatus::OCCUPIED) 
			{
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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
			}
			else if (Status == ETileStatus::OCCUPIED)
			{
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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
			}
			else if (Status == ETileStatus::OCCUPIED) 
			{
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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
			}
			else if (Status == ETileStatus::OCCUPIED) 
			{
				if (NextTile->GetOwner() == ETileOwner::BLACK)
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

	ApplyPossibleMovesMaterials(PossibleBishopMoves);
}

void ACHS_HumanPlayer::QueenPossibleMoves(FVector QueenLocation)
{

	PossibleQueenMoves.Empty();
	RookPossibleMoves(QueenLocation);
	BishopPossibleMoves(QueenLocation);
	PossibleQueenMoves.Append(PossibleRookMoves);
	PossibleQueenMoves.Append(PossibleBishopMoves);
}



>>>>>>> parent of 483c1e1 (Check starting implementatition)

void ACHS_HumanPlayer::SetOriginalTileMaterial() 
{

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	for (const auto& Entry : OriginalMaterials)
	{
		ATile* Tile = GameMode->GField->TileMap[Entry.Key];
		Tile->SetTileMaterial(0, Entry.Value);
	}

}

int32 ACHS_HumanPlayer::IsPieceMoveValid(FVector2D Position, TArray<FVector2D> PossiblePieceMoves)
{
	int32 ValidMove = 0;

	for (const FVector2D& PossibleMove : PossiblePieceMoves)
	{
		if (Position == PossibleMove)
		{
			ValidMove = 1;

		}
	}
	return ValidMove;


}

void ACHS_HumanPlayer::ApplyPossibleMovesMaterials(const TArray<FVector2D>& PossibleMoves)
{
	UMaterialInterface* TileMaterial;
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	for (const FVector2D& PossibleMove : PossibleMoves)
	{
		if (GameMode->GField->TileMap.Contains(PossibleMove))
		{
			ATile* Tile = GameMode->GField->TileMap[PossibleMove];
			OriginalMaterials.Add(PossibleMove, Tile->GetTileMaterial(0));

			if (Tile->GetOwner() != ETileOwner::WHITE)
			{
				TileMaterial = Tile->PossibleMoveMaterial;
				Tile->SetTileMaterial(0, TileMaterial);
			}
		}
	}
}
<<<<<<< HEAD


=======
>>>>>>> parent of 483c1e1 (Check starting implementatition)
