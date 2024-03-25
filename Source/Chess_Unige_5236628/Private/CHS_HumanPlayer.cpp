// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_HumanPlayer.h"
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

	GameInstance = Cast<UCHS_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

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

// Called after AI end turn
void ACHS_HumanPlayer::OnTurn()
{
	IsMyTurn = true;
	GameInstance->SetTurnMessage(TEXT("Human Player Turn"));

	// Check if white king is on check or on checkmate and calculate all the illegal moves that white can't do this turn
	IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
	IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);

}

// Called when user click in-game
void ACHS_HumanPlayer::OnClick()
{
	// GameMode
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	
	// Reset bool
	IsKillMove = false;

	// On second click tile material are reset
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
				// Click counter. First click on white pieces. Second click on empty tile or on black piece.
				ClickCounter = ClickCounter + 1;

				// Get clicked piece xyz location
				ClickedActorLocation = CurrPiece->GetActorLocation();

				// Save the first clicked piece for the future if conditions
				BasePieceActor = CurrPiece;

				// Check which type of piece is 
				if (BasePieceActor->IsA(AKnight::StaticClass()))
				{
					// Calculate his possible move 
					KnightPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					//Apply possible moves materials to tiles
					ApplyPossibleMovesMaterials(PossibleKnightMoves);
				}
				else if (BasePieceActor->IsA(AKing::StaticClass()))
				{
					KingPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(PossibleKingMoves);
				}
				else if (BasePieceActor->IsA(APawnChess::StaticClass()))
				{
					PawnPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(PossiblePawnMoves);
				}
				else if (BasePieceActor->IsA(ARook::StaticClass()))
				{
					RookPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(PossibleRookMoves);
				}
				else if (BasePieceActor->IsA(ABishop::StaticClass()))
				{
					BishopPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(PossibleBishopMoves);
				}
				else if (BasePieceActor->IsA(AQueen::StaticClass()))
				{
					QueenPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
					ApplyPossibleMovesMaterials(PossibleQueenMoves);
				}
			}
		}
	}
	// Second click
	if (Hit.bBlockingHit && IsMyTurn && ClickCounter != 0)
	{
		// Check if the second click is on a tile
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
		{
			FVector TileLocation = CurrTile->GetActorLocation();
			FVector2d Position(TileLocation);
			ETileStatus Status = CurrTile->GetTileStatus();

			// Check if the second click is on an empty tile
			if (Status == ETileStatus::EMPTY)
			{	
				// Check which type of piece is 
				if (BasePieceActor->IsA(APawnChess::StaticClass()))
				{
					// Check if the clicked tile is a legal move
					if (int32 IsValid = IsPieceMoveValid(Position, PossiblePawnMoves) == 1)
					{
						// Move white piece into clicked tile
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
					}
				}
				else if (BasePieceActor->IsA(AKing::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleKingMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
					
					}
				}
				else if (BasePieceActor->IsA(AQueen::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleQueenMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
					
					}
				}
				else if (BasePieceActor->IsA(ABishop::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleBishopMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
					}
				}
				else if (BasePieceActor->IsA(AKnight::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleKnightMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
					}
				}
				else if (BasePieceActor->IsA(ARook::StaticClass()))
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleRookMoves) == 1)
					{
						MoveBasePiece(BasePieceActor, ClickedActorLocation, TileLocation);
						
					}
				}
			}
		}
		// Check if second click is on an actor
		else if (ABasePiece* ClickedPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			EPieceColor Color = ClickedPiece->GetPieceColor();

			// Check if actor is black and if it's not a black king
			if (Color == EPieceColor::BLACK && !(ClickedPiece->IsA(AKing::StaticClass())))
			{
				FVector BlackActorLocation = ClickedPiece->GetActorLocation();
				FVector2D Position (BlackActorLocation);

				// It's a kill move. Information used by register
				IsKillMove = true;

				// Check piece type
				if (BasePieceActor->IsA(APawnChess::StaticClass())) 
				{
					// Check if click location is a legal move
					if (int32 IsValid = IsPieceMoveValid(Position, PossiblePawnMoves) == 1) 
					{
						// Set clicked black actor as hidden
						SetHiddenKilledPiece(Position, ClickedPiece);

						// Move white actor
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
				else if (BasePieceActor->IsA(AKing::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleKingMoves) == 1) 
					{
						SetHiddenKilledPiece(Position, ClickedPiece);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
				else if (BasePieceActor->IsA(AQueen::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleQueenMoves) == 1) 
					{
						SetHiddenKilledPiece(Position, ClickedPiece);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
				else if (BasePieceActor->IsA(ABishop::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleBishopMoves) == 1) 
					{
						SetHiddenKilledPiece(Position, ClickedPiece);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
				else if (BasePieceActor->IsA(AKnight::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleKnightMoves) == 1) 
					{
						SetHiddenKilledPiece(Position, ClickedPiece);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
				else if (BasePieceActor->IsA(ARook::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleRookMoves) == 1)
					{
						SetHiddenKilledPiece(Position, ClickedPiece);
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
			}
		}
	}
}

// Called when it's a kill move
void ACHS_HumanPlayer::SetHiddenKilledPiece(FVector2D Position, ABasePiece* ClickedPiece)
{
	Position.X = Position.X / 120;
	Position.Y = Position.Y / 120;

	// Set clicked piece hidden
	ClickedPiece->SetActorHiddenInGame(true);
	ClickedPiece->SetActorEnableCollision(false);

	//Remove killed piece from pieces map
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	GameMode->GField->BasePieceMap.Remove(Position);

	// New object for replay
	FDestroyedPiece NewDestroyedPiece;
	NewDestroyedPiece.Piece = ClickedPiece;
	NewDestroyedPiece.Position = Position;
	NewDestroyedPiece.TurnCounter = GameInstance->DestroyedPieceArrayIndexCounter;
	GameInstance->DestroyedPieceArray.Add(NewDestroyedPiece);
}

void ACHS_HumanPlayer::MoveBasePiece(ABasePiece*, FVector OldLocation, FVector NewLocation)
{
	// Move actor
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	BasePieceActor->SetActorLocation(NewLocation);

	FVector2D ClickedActorLocation2D(OldLocation);
	FVector NewActorLocation = BasePieceActor->GetActorLocation();

	// Add move for replay
	GameInstance->MovesForReplay.Add(NewActorLocation);
	GameInstance->PiecesForReplay.Add(BasePieceActor);

	FVector2D NewActorLocation2D(NewActorLocation);

	// Check if actor moved 
	if (NewActorLocation == NewLocation)
	{	
		// Update tiles status and owners
		GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
		GameMode->GField->TileMap[NewActorLocation2D]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);

	}

	// Reset click counter 
	ClickCounter = 0;
	// Update turn counter
	TurnCounter = TurnCounter + 1;

	ClickedActorLocation2D.X = ClickedActorLocation2D.X / 120;
	ClickedActorLocation2D.Y = ClickedActorLocation2D.Y / 120;
	NewActorLocation2D.X = NewActorLocation2D.X / 120;
	NewActorLocation2D.Y = NewActorLocation2D.Y / 120;

	// Reset register moves counter if it's the first move
	if (GameInstance->Moves.Num() == 0)
	{
		RegisterMovesCounter = 0;
	}

	// Gives moves information to register
	RegisterMoveConverter(FVector2D(NewActorLocation2D.X + 1, NewActorLocation2D.Y + 1), FVector2D(ClickedActorLocation2D.X + 1, ClickedActorLocation2D.Y + 1), BasePieceActor, ETileOwner::BLACK);
	
	// Remove moved piece from map 
	GameMode->GField->BasePieceMap.Remove(ClickedActorLocation2D);
	// Update map with the moved actor in his new location
	GameMode->GField->BasePieceMap.Add(NewActorLocation2D, BasePieceActor);
	
	// If it's a pawn in promotion position disable user input. A Trigger box in-game will open the promotion menu.
	if (BasePieceActor->IsA(APawnChess::StaticClass()) && NewActorLocation2D.X == 7)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

		PlayerController->DisableInput(PlayerController);

		// User selects promotion in-game ...
	}
	else
	{
		// Update turn counter
		GameInstance->DestroyedPieceArrayIndexCounter++;
		// End turn
		GameMode->EndHumanTurn();
	}
	
}

// Reset tile materials
void ACHS_HumanPlayer::SetOriginalTileMaterial() 
{

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	for (const auto& Entry : OriginalMaterials)
	{
		ATile* Tile = GameMode->GField->TileMap[Entry.Key];
		Tile->SetTileMaterial(0, Entry.Value);
	}

}

// Check if the move position given is a legal move
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

// Apply possible moves materials on tiles after first click on white piece
void ACHS_HumanPlayer::ApplyPossibleMovesMaterials(const TArray<FVector2D>& PossibleMoves)
{
	UMaterialInterface* TileMaterial;
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	for (const FVector2D& PossibleMove : PossibleMoves)
	{
		if (GameMode->GField->TileMap.Contains(PossibleMove))
		{	
			// Save actual tile material for the future reset
			ATile* Tile = GameMode->GField->TileMap[PossibleMove];
			OriginalMaterials.Add(PossibleMove, Tile->GetTileMaterial(0));

			// Check for safety
			if (Tile->GetOwner() != ETileOwner::WHITE)
			{
				// Apply new material
				TileMaterial = Tile->PossibleMoveMaterial;
				Tile->SetTileMaterial(0, TileMaterial);
			}
		}
	}
}

