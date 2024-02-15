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
		GameMode->GField->CheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
		GameMode->EndHumanTurn();

	}
}


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


