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
				
				ClickCounter = ClickCounter + 1;

				// Get clicked piece xyz location
				ClickedActorLocation = CurrPiece->GetActorLocation();

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
				else if (BasePieceActor->IsA(ARook::StaticClass())) {
					RookPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(ABishop::StaticClass())) {
					BishopPossibleMoves(ClickedActorLocation);
				}
				else if (BasePieceActor->IsA(AQueen::StaticClass())) {
					QueenPossibleMoves(ClickedActorLocation);
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

			if (BasePieceActor->IsA(APawnChess::StaticClass())) {
				if (int32 IsValid = IsPawnMoveValid(Position) == 1) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			else if (BasePieceActor->IsA(AKing::StaticClass())) {
				if (int32 IsValid = IsKingMoveValid(Position) == 1) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			else if (BasePieceActor->IsA(AQueen::StaticClass())) {
				if (int32 IsValid = IsQueenMoveValid(Position) == 1) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			else if (BasePieceActor->IsA(ABishop::StaticClass())) {
				if (int32 IsValid = IsBishopMoveValid(Position) == 1) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			else if (BasePieceActor->IsA(AKnight::StaticClass())) {
				if (int32 IsValid = IsKnightMoveValid(Position) == 1) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}

			else if (BasePieceActor->IsA(ARook::StaticClass())) {
				if (int32 IsValid = IsRookMoveValid(Position) == 1) {
					MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);

				}
			}
		}

		else if (ABasePiece* ClickedPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			EPieceColor Color = ClickedPiece->GetPieceColor();

			if (Color == EPieceColor::BLACK) {

				FVector BlackActorLocation = ClickedPiece->GetActorLocation();
				FVector2D Position (BlackActorLocation);

				if (BasePieceActor->IsA(APawnChess::StaticClass())) {
					if (int32 IsValid = IsPawnMoveValid(Position) == 1) {
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}
	
				else if (BasePieceActor->IsA(AKing::StaticClass())) {
					if (int32 IsValid = IsKingMoveValid(Position) == 1) {
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				else if (BasePieceActor->IsA(AQueen::StaticClass())) {
					if (int32 IsValid = IsQueenMoveValid(Position) == 1) {
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				else if (BasePieceActor->IsA(ABishop::StaticClass())) {
					if (int32 IsValid = IsBishopMoveValid(Position) == 1) {
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				else if (BasePieceActor->IsA(AKnight::StaticClass())) {
					if (int32 IsValid = IsKnightMoveValid(Position) == 1) {
						ClickedPiece->Destroy();
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
					}
				}

				else if (BasePieceActor->IsA(ARook::StaticClass())) {
					if (int32 IsValid = IsRookMoveValid(Position) == 1) {
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

void ACHS_HumanPlayer::KnightPossibleMoves(FVector KnightLocation) {

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	 PossibleKnightMoves = {
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

	PossibleKingMoves = {
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

	PossiblePawnMoves.Empty();

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

void ACHS_HumanPlayer::RookPossibleMoves(FVector RookLocation) {
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());

	UMaterialInterface* TileMaterial;
	PossibleRookMoves.Empty();

	for (int32 i = 1; i <= 7; ++i) {

		FVector2D NextTileLocation(RookLocation.X, RookLocation.Y + (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation)) {

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY) {
				PossibleRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED) {

				if (NextTile->GetOwner() == ETileOwner::BLACK) {
					PossibleRookMoves.Add(NextTileLocation); 
				}
				break; 
			}
			else {
				break; 
			}
		}
	}

	
	for (int32 i = 1; i <= 7; ++i) {

		FVector2D NextTileLocation(RookLocation.X, RookLocation.Y - (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation)) {

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY) {
				PossibleRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED) {
				if (NextTile->GetOwner() == ETileOwner::BLACK) {
					PossibleRookMoves.Add(NextTileLocation); 
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

		if (GameMode->GField->TileMap.Contains(NextTileLocation)) {

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY) {
				PossibleRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED) {

				if (NextTile->GetOwner() == ETileOwner::BLACK) {
					PossibleRookMoves.Add(NextTileLocation); 
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


	for (int32 i = 1; i <= 7; ++i) {

		FVector2D NextTileLocation(RookLocation.X - (i * 120.0), RookLocation.Y);

		if (GameMode->GField->TileMap.Contains(NextTileLocation)) {

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY) {
				PossibleRookMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED) {

				if (NextTile->GetOwner() == ETileOwner::BLACK) {
					PossibleRookMoves.Add(NextTileLocation); 
				}
				break; 
			}
			else {
				break; 
			}
		}
	}

	for (const FVector2D& PossibleMove : PossibleRookMoves) {
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

void ACHS_HumanPlayer::BishopPossibleMoves(FVector BishopLocation) {
	ACHS_GameMode* GameMode = Cast<ACHS_GameMode>(GetWorld()->GetAuthGameMode());
	

	UMaterialInterface* TileMaterial;
	PossibleBishopMoves.Empty();

	
	for (int32 i = 1; i <= 7; ++i) {

		FVector2D NextTileLocation(BishopLocation.X + (i * 120.0f), BishopLocation.Y + (i * 120.0f));

		if (GameMode->GField->TileMap.Contains(NextTileLocation)) {

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY) {
				PossibleBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED) {
				if (NextTile->GetOwner() == ETileOwner::BLACK) {
					PossibleBishopMoves.Add(NextTileLocation); 
				}
				break; 
			}
			else {
				break; 
			}
		}
	}

	
	for (int32 i = 1; i <= 7; ++i) {

		FVector2D NextTileLocation(BishopLocation.X + (i * 120.0), BishopLocation.Y - (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation)) {

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY) {
				PossibleBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED) {
				if (NextTile->GetOwner() == ETileOwner::BLACK) {

					PossibleBishopMoves.Add(NextTileLocation); 
				}
				break; 
			}
			else {
				break; 
			}
		}
	}

	
	for (int32 i = 1; i <= 7; ++i) {

		FVector2D NextTileLocation(BishopLocation.X - (i * 120.0), BishopLocation.Y + (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation)) {

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY) {
				PossibleBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED) {

				if (NextTile->GetOwner() == ETileOwner::BLACK) {
					PossibleBishopMoves.Add(NextTileLocation);
				}
				break; 
			}
			else {
				break; 
			}
		}
	}

	
	for (int32 i = 1; i <= 7; ++i) {

		FVector2D NextTileLocation(BishopLocation.X - (i * 120.0), BishopLocation.Y - (i * 120.0));

		if (GameMode->GField->TileMap.Contains(NextTileLocation)) {

			ATile* NextTile = GameMode->GField->TileMap[NextTileLocation];
			ETileStatus Status = NextTile->GetTileStatus();

			if (Status == ETileStatus::EMPTY) {
				PossibleBishopMoves.Add(NextTileLocation);
			}
			else if (Status == ETileStatus::OCCUPIED) {

				if (NextTile->GetOwner() == ETileOwner::BLACK) {
					PossibleBishopMoves.Add(NextTileLocation); 
				}
				break; 
			}
			else {
				break; 
			}
		}
	}

	for (const FVector2D& PossibleMove : PossibleBishopMoves) {
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

void ACHS_HumanPlayer::QueenPossibleMoves(FVector QueenLocation) {

	PossibleQueenMoves.Empty();
	RookPossibleMoves(QueenLocation);
	BishopPossibleMoves(QueenLocation);
	PossibleQueenMoves.Append(PossibleRookMoves);
	PossibleQueenMoves.Append(PossibleBishopMoves);
}




void ACHS_HumanPlayer::SetOriginalTileMaterial() {

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	for (const auto& Entry : OriginalMaterials) {
		ATile* Tile = GameMode->GField->TileMap[Entry.Key];
		Tile->SetTileMaterial(0, Entry.Value);
	}

}

int32 ACHS_HumanPlayer::IsPawnMoveValid(FVector2D Position) {
	int32 ValidMove = 0;
	for (const FVector2D& PossibleMove : PossiblePawnMoves) {
		if (Position == PossibleMove) {
			ValidMove = 1;
			
		}
	}
return ValidMove;
}


int32 ACHS_HumanPlayer::IsBishopMoveValid(FVector2D Position) {
	int32 ValidMove = 0;
	for (const FVector2D& PossibleMove : PossibleBishopMoves) {
		if (Position == PossibleMove) {
			ValidMove = 1;

		}
	}
	return ValidMove;
}

int32 ACHS_HumanPlayer::IsKnightMoveValid(FVector2D Position) {
	int32 ValidMove = 0;
	for (const FVector2D& PossibleMove : PossibleKnightMoves) {
		if (Position == PossibleMove) {
			ValidMove = 1;

		}
	}
	return ValidMove;
}

int32 ACHS_HumanPlayer::IsQueenMoveValid(FVector2D Position) {
	int32 ValidMove = 0;
	for (const FVector2D& PossibleMove : PossibleQueenMoves) {
		if (Position == PossibleMove) {
			ValidMove = 1;

		}
	}
	return ValidMove;
}

int32 ACHS_HumanPlayer::IsKingMoveValid(FVector2D Position) {
	int32 ValidMove = 0;
	for (const FVector2D& PossibleMove : PossibleKingMoves) {
		if (Position == PossibleMove) {
			ValidMove = 1;

		}
	}
	return ValidMove;
}

int32 ACHS_HumanPlayer::IsRookMoveValid(FVector2D Position) {
	int32 ValidMove = 0;
	for (const FVector2D& PossibleMove : PossibleRookMoves) {
		if (Position == PossibleMove) {
			ValidMove = 1;

		}
	}
	return ValidMove;
}