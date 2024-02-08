// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_HumanPlayer.h"
#include "GameField.h"
#include "BasePiece.h"
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
			if (Color == EPieceColor::WHITE) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked"));
				ClickCounter = ClickCounter + 1;
				// Get clicked piece xyz location
				ClickedActorLocation = CurrPiece->GetActorLocation();
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Piece %s"), *ClickedActorLocation.ToString()));
				// Save the first clicked piece for the future if conditions
				BasePieceActor = CurrPiece;
				if (BasePieceActor->IsA(AKnight::StaticClass())) {
					KnightPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(AKing::StaticClass())) {
					KingPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(APawnChess::StaticClass())) {
					PawnPossibleMoves(ClickedActorLocation);
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
			ETileStatus Status = CurrTile->GetTileStatus();

			// START PAWN TO EMPTY TILES MOVES SECTION
			// Check if the tile is empty, if the first clicked actor was a pawn and if it's a legal move for the pawn
			if (BasePieceActor->IsA(APawnChess::StaticClass())) {
				if (int32 Occupied = PawnIsValidMovement(ClickedActorLocation, TileLocation, 0) == 0) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			//START TOWER TO EMPTY TILE MOVES
			if (BasePieceActor->IsA(ARook::StaticClass())) {
				if (int32 Occupied = RookIsValidMovement(ClickedActorLocation, TileLocation, 0) == 0) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
				}
			}

			//START TOWER TO EMPTY TILE MOVES
			if (BasePieceActor->IsA(ABishop::StaticClass())) {
				if (int32 Occupied = BishopIsValidMovement(ClickedActorLocation, TileLocation, 0) == 0) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			//START TOWER TO EMPTY TILE MOVES
			if (BasePieceActor->IsA(AQueen::StaticClass())) {
				if (int32 Occupied = QueenIsValidMovement(ClickedActorLocation, TileLocation, 0) == 0) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			//START TOWER TO EMPTY TILE MOVES
			if (BasePieceActor->IsA(AKing::StaticClass())) {
				if (int32 Occupied = KingIsValidMovement(ClickedActorLocation, TileLocation) == 0) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			//START TOWER TO EMPTY TILE MOVES
			if (BasePieceActor->IsA(AKnight::StaticClass())) {
				if (int32 Occupied = KnightIsValidMovement(ClickedActorLocation, TileLocation) == 0) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}
		}

		if (ABasePiece* ClickedPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			EPieceColor Color = ClickedPiece->GetPieceColor();

			if (Color == EPieceColor::BLACK) {

				FVector BlackActorLocation = ClickedPiece->GetActorLocation();

				// START PAWN TO OCCUPIED TILES MOVES SECTION
				if (BasePieceActor->IsA(APawnChess::StaticClass())) {
					if (int32 Occupied = PawnIsValidMovement(ClickedActorLocation, BlackActorLocation, 1) == 0) {
						GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BLACK %s"), *BlackActorLocation.ToString()));
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				//START ROOK TO OCCUPIED TILES MOVES SECTION
				if (BasePieceActor->IsA(ARook::StaticClass())) {
					if (int32 Occupied = RookIsValidMovement(ClickedActorLocation, BlackActorLocation, 1) == 0) {
						GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BLACK %s"), *BlackActorLocation.ToString()));
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				//START ROOK TO OCCUPIED TILES MOVES SECTION
				if (BasePieceActor->IsA(ABishop::StaticClass())) {
					if (int32 Occupied = BishopIsValidMovement(ClickedActorLocation, BlackActorLocation, 1) == 0) {
						GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BLACK %s"), *BlackActorLocation.ToString()));
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				//START ROOK TO OCCUPIED TILES MOVES SECTION
				if (BasePieceActor->IsA(AQueen::StaticClass())) {
					if (int32 Occupied = QueenIsValidMovement(ClickedActorLocation, BlackActorLocation, 1) == 0) {
						GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BLACK %s"), *BlackActorLocation.ToString()));
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				//START ROOK TO OCCUPIED TILES MOVES SECTION
				if (BasePieceActor->IsA(AKing::StaticClass())) {
					if (int32 Occupied = KingIsValidMovement(ClickedActorLocation, BlackActorLocation) == 0) {
						GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BLACK %s"), *BlackActorLocation.ToString()));
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				//START ROOK TO OCCUPIED TILES MOVES SECTION
				if (BasePieceActor->IsA(AKnight::StaticClass())) {
					if (int32 Occupied = KnightIsValidMovement(ClickedActorLocation, BlackActorLocation) == 0) {
						GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BLACK %s"), *BlackActorLocation.ToString()));
						ClickedPiece->Destroy();
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

void ACHS_HumanPlayer::MoveBasePiece(ABasePiece*, FVector OldLocation, FVector NewLocation) {

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	BasePieceActor->SetActorLocation(NewLocation);
	GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("FREE %s"), *NewLocation.ToString()));
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
	//IsMyTurn = false;


}

int32 ACHS_HumanPlayer::RookIsValidMovement(FVector FirstClickedActorLocation, FVector TileLocation, int32 ToTileOrToBlack)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 Occupied = 1;

	if (TileLocation.X == FirstClickedActorLocation.X) {
		if ((FirstClickedActorLocation.Y - TileLocation.Y) < 0) {

			Occupied = 0;

			if (ToTileOrToBlack == 1) {
				TileLocation.Y = TileLocation.Y - 120;
			}
			for (FVector2D i(FirstClickedActorLocation.X, FirstClickedActorLocation.Y + 120); i.Y <= TileLocation.Y; i.Y = i.Y + 120) {
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("I TEST %s"), *i.ToString()));
				ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();

				if (status == ETileStatus::OCCUPIED) {
					Occupied = 1;
				}
			}
		}
		if ((FirstClickedActorLocation.Y - TileLocation.Y) > 0) {

			Occupied = 0;

			if (ToTileOrToBlack == 1) {
				TileLocation.Y = TileLocation.Y + 120;
			}
			for (FVector2D i(FirstClickedActorLocation.X, FirstClickedActorLocation.Y - 120); i.Y >= TileLocation.Y; i.Y = i.Y - 120) {
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("I TEST %s"), *i.ToString()));
				ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();

				if (status == ETileStatus::OCCUPIED) {
					Occupied = 1;
				}
			}
		}

	}
	else if(TileLocation.Y == FirstClickedActorLocation.Y){

		if ((FirstClickedActorLocation.X - TileLocation.X) < 0) {

			Occupied = 0;

			if (ToTileOrToBlack == 1) {
				TileLocation.X = TileLocation.X - 120;
			}
			for (FVector2D i(FirstClickedActorLocation.X + 120, FirstClickedActorLocation.Y); i.X <= TileLocation.X; i.X = i.X + 120) {
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("I TEST %s"), *i.ToString()));
				ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();

				if (status == ETileStatus::OCCUPIED) {
					Occupied = 1;
				}
			}
		}

		if ((FirstClickedActorLocation.X - TileLocation.X) > 0) {

			Occupied = 0;

			if (ToTileOrToBlack == 1) {
				TileLocation.X = TileLocation.X + 120;
			}

			for (FVector2D i(FirstClickedActorLocation.X - 120, FirstClickedActorLocation.Y); i.X >= TileLocation.X; i.X = i.X - 120) {
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("I TEST %s"), *i.ToString()));
				ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();

				if (status == ETileStatus::OCCUPIED) {
					Occupied = 1;
				}
			}
		}
	}
	return Occupied;
}

int32 ACHS_HumanPlayer::PawnIsValidMovement(FVector FirstClickedActorLocation, FVector TileLocation, int32 ToTileOrToBlack) 
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 Occupied = 1;
	if (ToTileOrToBlack == 0) {
		if ((FirstClickedActorLocation.X == 120.0 && TileLocation.X == 360.0 || TileLocation.X - FirstClickedActorLocation.X == 120.0) && TileLocation.Y == FirstClickedActorLocation.Y) {
			if ((FirstClickedActorLocation.X - TileLocation.X) < 0) {

				Occupied = 0;

				for (FVector2D i(FirstClickedActorLocation.X + 120, FirstClickedActorLocation.Y); i.X <= TileLocation.X; i.X = i.X + 120) {
					GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("I TEST %s"), *i.ToString()));
					ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();

					if (status == ETileStatus::OCCUPIED) {
						Occupied = 1;
					}
				}
			}
		}
	}
	else
	{
		if (TileLocation.X - FirstClickedActorLocation.X == 120.0 && (TileLocation.Y - FirstClickedActorLocation.Y == 120.0 || TileLocation.Y - FirstClickedActorLocation.Y == -120.0)) {

			Occupied = 0;
		}
	}

	return Occupied;
	

}

int32 ACHS_HumanPlayer::BishopIsValidMovement(FVector FirstClickedActorLocation, FVector TileLocation, int32 ToTileOrToBlack)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 Occupied = 1;

	
	if (FMath::Abs(TileLocation.X - FirstClickedActorLocation.X) == FMath::Abs(TileLocation.Y - FirstClickedActorLocation.Y)) {
		
		if (TileLocation.X > FirstClickedActorLocation.X && TileLocation.Y > FirstClickedActorLocation.Y) {
			Occupied = 0;
			if (ToTileOrToBlack == 1) {
				TileLocation.X = TileLocation.X - 120;
				TileLocation.Y = TileLocation.Y - 120;
			}
			for (FVector2D i(FirstClickedActorLocation.X + 120, FirstClickedActorLocation.Y + 120); i.X <= TileLocation.X && i.Y <= TileLocation.Y; i.X = i.X + 120, i.Y = i.Y + 120) {
				ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();
				if (status == ETileStatus::OCCUPIED) {
					Occupied = 1;
				}
			}
		}
		
		else if (TileLocation.X < FirstClickedActorLocation.X && TileLocation.Y > FirstClickedActorLocation.Y) {
			Occupied = 0;
			if (ToTileOrToBlack == 1) {
				TileLocation.X = TileLocation.X + 120;
				TileLocation.Y = TileLocation.Y - 120;
			}
			for (FVector2D i(FirstClickedActorLocation.X - 120, FirstClickedActorLocation.Y + 120); i.X >= TileLocation.X && i.Y <= TileLocation.Y; i.X = i.X - 120, i.Y = i.Y + 120) {
				ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();
				if (status == ETileStatus::OCCUPIED) {
					Occupied = 1;
				}
			}
		}
		
		else if (TileLocation.X < FirstClickedActorLocation.X && TileLocation.Y < FirstClickedActorLocation.Y) {
			Occupied = 0;
			if (ToTileOrToBlack == 1) {
				TileLocation.X = TileLocation.X + 120;
				TileLocation.Y = TileLocation.Y + 120;
			}
			for (FVector2D i(FirstClickedActorLocation.X - 120, FirstClickedActorLocation.Y - 120); i.X >= TileLocation.X && i.Y >= TileLocation.Y; i.X = i.X - 120, i.Y = i.Y - 120) {
				ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();
				if (status == ETileStatus::OCCUPIED) {
					Occupied = 1;
				}
			}
		}
		
		else if (TileLocation.X > FirstClickedActorLocation.X && TileLocation.Y < FirstClickedActorLocation.Y) {
			Occupied = 0;
			if (ToTileOrToBlack == 1) {
				TileLocation.X = TileLocation.X - 120;
				TileLocation.Y = TileLocation.Y + 120;
			}
			for (FVector2D i(FirstClickedActorLocation.X + 120, FirstClickedActorLocation.Y - 120); i.X <= TileLocation.X && i.Y >= TileLocation.Y; i.X = i.X + 120, i.Y = i.Y - 120) {
				ETileStatus status = GameMode->GField->TileMap[i]->GetTileStatus();
				if (status == ETileStatus::OCCUPIED) {
					Occupied = 1;
				}
			}
		}
	}

	return Occupied;
}


int32 ACHS_HumanPlayer::QueenIsValidMovement(FVector FirstClickedActorLocation, FVector TileLocation, int32 ToTileOrToBlack) {

	int32 Occupied = 1;

	if (TileLocation.X == FirstClickedActorLocation.X || TileLocation.Y == FirstClickedActorLocation.Y) 
	{
		Occupied = RookIsValidMovement(FirstClickedActorLocation, TileLocation, ToTileOrToBlack);
	}
	else {

		Occupied = BishopIsValidMovement(FirstClickedActorLocation, TileLocation, ToTileOrToBlack);
	}
	
	return Occupied;

}

int32 ACHS_HumanPlayer::KingIsValidMovement(FVector FirstClickedActorLocation, FVector TileLocation)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 Occupied = 1;

	if (FMath::Abs(TileLocation.X - FirstClickedActorLocation.X) <= 120 && FMath::Abs(TileLocation.Y - FirstClickedActorLocation.Y) <= 120) {
		Occupied = 0;
	}

	return Occupied;
}

int32 ACHS_HumanPlayer::KnightIsValidMovement(FVector FirstClickedActorLocation, FVector TileLocation)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 Occupied = 1;

	TArray<FVector2D> PossibleKnightMoves = {
		FVector2D(FirstClickedActorLocation.X + 120, FirstClickedActorLocation.Y + 240),
		FVector2D(FirstClickedActorLocation.X + 120, FirstClickedActorLocation.Y - 240),
		FVector2D(FirstClickedActorLocation.X - 120, FirstClickedActorLocation.Y + 240),
		FVector2D(FirstClickedActorLocation.X - 120, FirstClickedActorLocation.Y - 240),
		FVector2D(FirstClickedActorLocation.X + 240, FirstClickedActorLocation.Y + 120),
		FVector2D(FirstClickedActorLocation.X + 240, FirstClickedActorLocation.Y - 120),
		FVector2D(FirstClickedActorLocation.X - 240, FirstClickedActorLocation.Y + 120),
		FVector2D(FirstClickedActorLocation.X - 240, FirstClickedActorLocation.Y - 120)
	};

	
	for (const FVector2D& PossibleMove : PossibleKnightMoves) {
		if (FVector2D(TileLocation) == PossibleMove) {
			Occupied = 0;
			break;
		}
	}


	return Occupied;
}

void ACHS_HumanPlayer::KnightPossibleMoves(FVector KnightLocation) {

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	TArray<FVector2D> PossibleKnightMoves = {
		FVector2D(KnightLocation.X + 120, KnightLocation.Y + 240),
		FVector2D(KnightLocation.X + 120, KnightLocation.Y - 240),
		FVector2D(KnightLocation.X - 120, KnightLocation.Y + 240),
		FVector2D(KnightLocation.X - 120, KnightLocation.Y - 240),
		FVector2D(KnightLocation.X + 240, KnightLocation.Y + 120),
		FVector2D(KnightLocation.X + 240, KnightLocation.Y - 120),
		FVector2D(KnightLocation.X - 240, KnightLocation.Y + 120),
		FVector2D(KnightLocation.X - 240, KnightLocation.Y - 120)
	};

	UMaterialInterface* TileMaterial;
	

	for (const FVector2D& PossibleMove : PossibleKnightMoves) {
		if (GameMode->GField->TileMap.Contains(PossibleMove)) {
			ATile* Tile = GameMode->GField->TileMap[PossibleMove];
			OriginalMaterials.Add(PossibleMove, Tile->GetTileMaterial(0));

			if (Tile->GetOwner() != ETileOwner::WHITE) {


				TileMaterial = Tile->PossibleMoveMaterial;
				Tile->SetTileMaterial(0, TileMaterial);
			}
		}
	}
}

void ACHS_HumanPlayer::KingPossibleMoves(FVector KingLocation) {

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	TArray<FVector2D> PossibleKingMoves = {
		FVector2D(KingLocation.X + 120, KingLocation.Y + 120),
		FVector2D(KingLocation.X + 120, KingLocation.Y - 120),
		FVector2D(KingLocation.X - 120, KingLocation.Y + 120),
		FVector2D(KingLocation.X - 120, KingLocation.Y - 120),
		FVector2D(KingLocation.X, KingLocation.Y + 120),
		FVector2D(KingLocation.X, KingLocation.Y - 120),
		FVector2D(KingLocation.X + 120, KingLocation.Y),
		FVector2D(KingLocation.X - 120, KingLocation.Y)
	};

	UMaterialInterface* TileMaterial;


	for (const FVector2D& PossibleMove : PossibleKingMoves) {
		if (GameMode->GField->TileMap.Contains(PossibleMove)) {
			ATile* Tile = GameMode->GField->TileMap[PossibleMove];
			OriginalMaterials.Add(PossibleMove, Tile->GetTileMaterial(0));

			if (Tile->GetOwner() != ETileOwner::WHITE) {


				TileMaterial = Tile->PossibleMoveMaterial;
				Tile->SetTileMaterial(0, TileMaterial);
			}
		}
	}
}

void ACHS_HumanPlayer::PawnPossibleMoves(FVector PawnLocation) {

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	UMaterialInterface* TileMaterial;

	TArray<FVector2D> PossiblePawnMoves;

	FVector2D Pawn2dLocation0(PawnLocation.X + 120, PawnLocation.Y);
	if (GameMode->GField->TileMap.Contains(Pawn2dLocation0)) {
		ETileStatus status0 = GameMode->GField->TileMap[Pawn2dLocation0]->GetTileStatus();
		if (status0 == ETileStatus::EMPTY) {

			PossiblePawnMoves.Add(FVector2D(PawnLocation.X + 120.0, PawnLocation.Y));
		}
	}
	FVector2D Pawn2dLocation3(PawnLocation.X + 240, PawnLocation.Y);
	if (GameMode->GField->TileMap.Contains(Pawn2dLocation3)) {
		ETileStatus status1 = GameMode->GField->TileMap[Pawn2dLocation3]->GetTileStatus();

		if (PawnLocation.X == 120.0 && status1 == ETileStatus::EMPTY) {

			PossiblePawnMoves.Add(FVector2D(PawnLocation.X + 240.0, PawnLocation.Y));
		}
	}
	FVector2D Pawn2dLocation1(PawnLocation.X + 120, PawnLocation.Y + 120);
	if (GameMode->GField->TileMap.Contains(Pawn2dLocation1)) {
		ETileOwner Owner0 = GameMode->GField->TileMap[Pawn2dLocation1]->GetOwner();

		if (Owner0 == ETileOwner::BLACK) {

			PossiblePawnMoves.Add(FVector2D(PawnLocation.X + 120.0, PawnLocation.Y + 120.0));
		}
	}
	FVector2D Pawn2dLocation2(PawnLocation.X + 120, PawnLocation.Y - 120);
	if (GameMode->GField->TileMap.Contains(Pawn2dLocation2)) {
		ETileOwner Owner1 = GameMode->GField->TileMap[Pawn2dLocation2]->GetOwner();

		if (Owner1 == ETileOwner::BLACK) {

			PossiblePawnMoves.Add(FVector2D(PawnLocation.X + 120.0, PawnLocation.Y - 120.0));
		}
	}
	for (const FVector2D& PossibleMove : PossiblePawnMoves) {
		if (GameMode->GField->TileMap.Contains(PossibleMove)) {
			ATile* Tile = GameMode->GField->TileMap[PossibleMove];
			OriginalMaterials.Add(PossibleMove, Tile->GetTileMaterial(0));

			if(Tile->GetOwner() != ETileOwner::WHITE) {

				TileMaterial = Tile->PossibleMoveMaterial;
				Tile->SetTileMaterial(0, TileMaterial);
			}
		}
	}

}



void ACHS_HumanPlayer::SetOriginalTileMaterial() {

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	for (const auto& Entry : OriginalMaterials) {
		ATile* Tile = GameMode->GField->TileMap[Entry.Key];
		Tile->SetTileMaterial(0, Entry.Value);
	}

}
