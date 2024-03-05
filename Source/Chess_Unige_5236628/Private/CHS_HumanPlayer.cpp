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

void ACHS_HumanPlayer::OnClick()
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	
	IsKillMove = false;

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
				

				IsCheckKing(ETileOwner::WHITE, ETileOwner::BLACK);
				IsCheckMate(ETileOwner::WHITE, ETileOwner::BLACK);


				/*for (const FVector2D& Vec : IllegalPawnMoveDueToCheck)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, VecAsString);
				}
				for (const FVector2D& Vec : IllegalKingMoveDueToCheck)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, VecAsString);
				}
				for (const FVector2D& Vec : IllegalBishopMoveDueToCheck)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, VecAsString);
				}

				for (const FVector2D& Vec : IllegalRookMoveDueToCheck)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Blue, VecAsString);
				}
				for (const FVector2D& Vec : IllegalKnightMoveDueToCheck)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Purple, VecAsString);
				}
				for (const FVector2D& Vec : IllegalQueenMoveDueToCheck)
				{
					FString VecAsString = FString::Printf(TEXT("X: %f, Y: %f"), Vec.X, Vec.Y);
					GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Orange, VecAsString);
				}*/

				
				
					ClickCounter = ClickCounter + 1;

					// Get clicked piece xyz location
					ClickedActorLocation = CurrPiece->GetActorLocation();

					// Save the first clicked piece for the future if conditions
					BasePieceActor = CurrPiece;

					if (BasePieceActor->IsA(AKnight::StaticClass()))
					{
						KnightPossibleMoves(ClickedActorLocation, ETileOwner::BLACK);
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
					if (int32 IsValid = IsPieceMoveValid(Position, PossiblePawnMoves) == 1)
					{
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
		else if (ABasePiece* ClickedPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			EPieceColor Color = ClickedPiece->GetPieceColor();

			if (Color == EPieceColor::BLACK && !(ClickedPiece->IsA(AKing::StaticClass())))
			{

				FVector BlackActorLocation = ClickedPiece->GetActorLocation();
				FVector2D Position (BlackActorLocation);
				IsKillMove = true;

				if (BasePieceActor->IsA(APawnChess::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossiblePawnMoves) == 1) 
					{
						Position.X = Position.X / 120;
						Position.Y = Position.Y / 120;

						ClickedPiece->SetActorHiddenInGame(true);
						ClickedPiece->SetActorEnableCollision(false);
						GameMode->GField->BasePieceMap.Remove(Position);
						FDestroyedPiece NewDestroyedPiece;
						NewDestroyedPiece.Piece = ClickedPiece;
						NewDestroyedPiece.Position = Position;
						GameInstance->DestroyedPieceArray.SetNum(GameInstance->DestroyedPieceArrayIndexCounter+1);
						GameInstance->DestroyedPieceArray[GameInstance->DestroyedPieceArrayIndexCounter] = NewDestroyedPiece;
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}
	
				else if (BasePieceActor->IsA(AKing::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleKingMoves) == 1) 
					{
						Position.X = Position.X / 120;
						Position.Y = Position.Y / 120;

						ClickedPiece->SetActorHiddenInGame(true);
						ClickedPiece->SetActorEnableCollision(false);
						GameMode->GField->BasePieceMap.Remove(Position);
						FDestroyedPiece NewDestroyedPiece;
						NewDestroyedPiece.Piece = ClickedPiece;
						NewDestroyedPiece.Position = Position;
						GameInstance->DestroyedPieceArray.SetNum(GameInstance->DestroyedPieceArrayIndexCounter+1);
						GameInstance->DestroyedPieceArray[GameInstance->DestroyedPieceArrayIndexCounter] = NewDestroyedPiece;
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}

				else if (BasePieceActor->IsA(AQueen::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleQueenMoves) == 1) 
					{
						Position.X = Position.X / 120;
						Position.Y = Position.Y / 120;

						ClickedPiece->SetActorHiddenInGame(true);
						ClickedPiece->SetActorEnableCollision(false);
						GameMode->GField->BasePieceMap.Remove(Position);
						FDestroyedPiece NewDestroyedPiece;
						NewDestroyedPiece.Piece = ClickedPiece;
						NewDestroyedPiece.Position = Position;
						GameInstance->DestroyedPieceArray.SetNum(GameInstance->DestroyedPieceArrayIndexCounter+1);
						GameInstance->DestroyedPieceArray[GameInstance->DestroyedPieceArrayIndexCounter] = NewDestroyedPiece;
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}

				else if (BasePieceActor->IsA(ABishop::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleBishopMoves) == 1) 
					{
						Position.X = Position.X / 120;
						Position.Y = Position.Y / 120;

						ClickedPiece->SetActorHiddenInGame(true);
						ClickedPiece->SetActorEnableCollision(false);
						GameMode->GField->BasePieceMap.Remove(Position);
						FDestroyedPiece NewDestroyedPiece;
						NewDestroyedPiece.Piece = ClickedPiece;
						NewDestroyedPiece.Position = Position;
						GameInstance->DestroyedPieceArray.SetNum(GameInstance->DestroyedPieceArrayIndexCounter+1);
						GameInstance->DestroyedPieceArray[GameInstance->DestroyedPieceArrayIndexCounter] = NewDestroyedPiece;
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}

				else if (BasePieceActor->IsA(AKnight::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleKnightMoves) == 1) 
					{
						Position.X = Position.X / 120;
						Position.Y = Position.Y / 120;

						ClickedPiece->SetActorHiddenInGame(true);
						ClickedPiece->SetActorEnableCollision(false);
						GameMode->GField->BasePieceMap.Remove(Position);
						FDestroyedPiece NewDestroyedPiece;
						NewDestroyedPiece.Piece = ClickedPiece;
						NewDestroyedPiece.Position = Position;
						GameInstance->DestroyedPieceArray.SetNum(GameInstance->DestroyedPieceArrayIndexCounter+1);
						GameInstance->DestroyedPieceArray[GameInstance->DestroyedPieceArrayIndexCounter] = NewDestroyedPiece;
						MoveBasePiece(BasePieceActor, ClickedActorLocation, BlackActorLocation);
						
					}
				}

				else if (BasePieceActor->IsA(ARook::StaticClass())) 
				{
					if (int32 IsValid = IsPieceMoveValid(Position, PossibleRookMoves) == 1)
					{
						Position.X = Position.X / 120;
						Position.Y = Position.Y / 120;

						ClickedPiece->SetActorHiddenInGame(true);
						ClickedPiece->SetActorEnableCollision(false);
						GameMode->GField->BasePieceMap.Remove(Position);
						FDestroyedPiece NewDestroyedPiece;
						NewDestroyedPiece.Piece = ClickedPiece;
						NewDestroyedPiece.Position = Position;
						GameInstance->DestroyedPieceArray.SetNum(GameInstance->DestroyedPieceArrayIndexCounter+1);
						GameInstance->DestroyedPieceArray[GameInstance->DestroyedPieceArrayIndexCounter] = NewDestroyedPiece;
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
	GameInstance->SetTurnMessage(TEXT("Human Player Turn"));
	
}

void ACHS_HumanPlayer::MoveBasePiece(ABasePiece*, FVector OldLocation, FVector NewLocation)
{

	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
	BasePieceActor->SetActorLocation(NewLocation);
	FVector2D ClickedActorLocation2D(OldLocation);
	FVector NewActorLocation = BasePieceActor->GetActorLocation();

	GameInstance->MovesForReplay.Add(NewActorLocation);
	GameInstance->PiecesForReplay.Add(BasePieceActor);

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

	if (GameInstance->Moves.Num() == 0)
	{
		RegisterMovesCounter = 0;
	}
	RegisterMoveConverter(FVector2D(NewActorLocation2D.X + 1, NewActorLocation2D.Y + 1), FVector2D(ClickedActorLocation2D.X + 1, ClickedActorLocation2D.Y + 1), BasePieceActor, ETileOwner::BLACK);
	
	

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
		GameInstance->DestroyedPieceArrayIndexCounter++;
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

