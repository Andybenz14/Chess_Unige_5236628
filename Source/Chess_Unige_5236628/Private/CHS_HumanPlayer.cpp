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
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);

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
				ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
				ClickCounter = ClickCounter + 1;
				// Get clicked piece xyz location
				ClickedActorLocation = CurrPiece->GetActorLocation();
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Piece %s"), *ClickedActorLocation.ToString()));
				// Save the first clicked piece for the future if conditions
				BasePieceActor = CurrPiece;
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
			if (Status == ETileStatus::EMPTY && TileLocation.X - ClickedActorLocation.X <= 240.0f && TileLocation.Y == ClickedActorLocation.Y && TurnCounter == 0 && BasePieceActor->IsA(APawnChess::StaticClass())) {

				//Get clicked piece xyz location
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Piece %s"), *TileLocation.ToString()));
				BasePieceActor->SetActorLocation(TileLocation);
				FVector2D ClickedActorLocation2D(ClickedActorLocation);
				FVector NewActorLocation = BasePieceActor->GetActorLocation();
				FVector2D NewActorLocation2D(NewActorLocation);

				// Check if the actor is in the new position. If yes sets the new tiles status.
				if (NewActorLocation == TileLocation) {
					ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
					GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
					GameMode->GField->TileMap[NewActorLocation2D]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
				}

				ClickCounter = 0;
				TurnCounter = TurnCounter + 1; //TODO al momento si può muovere solo al primo turno di 2, ma per il futuro deve essere la prima mosse per ogni pawn non del player
				//IsMyTurn = false;
			}
			else if (Status == ETileStatus::EMPTY && TileLocation.X - ClickedActorLocation.X == 120.0f && TileLocation.Y == ClickedActorLocation.Y && BasePieceActor->IsA(APawnChess::StaticClass())) {
				//Get clicked piece xyz location
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Tile %s"), *TileLocation.ToString()));
				BasePieceActor->SetActorLocation(TileLocation);
				FVector2D ClickedActorLocation2D(ClickedActorLocation);
				FVector NewActorLocation = BasePieceActor->GetActorLocation();
				FVector2D NewActorLocation2D(NewActorLocation);

				if (NewActorLocation == TileLocation) {
					ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
					GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
					GameMode->GField->TileMap[NewActorLocation2D]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
				}

				ClickCounter = 0;
				TurnCounter = TurnCounter + 1;
				//IsMyTurn = false;
			}
			//END OF PAWN TO EMPTY TILES MOVES SECTION

			//START TOWER TO EMPTY TILE MOVES
			// Check if the tile is empty, if the first clicked actor was a rook and if it's a legal move for the rook
			else if (Status == ETileStatus::EMPTY && (TileLocation.X == ClickedActorLocation.X || TileLocation.Y == ClickedActorLocation.Y) && BasePieceActor->IsA(ARook::StaticClass())) {
				
				//Get clicked piece xyz location
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Tile %s"), *TileLocation.ToString()));
				BasePieceActor->SetActorLocation(TileLocation, true, nullptr, ETeleportType::TeleportPhysics);
				FVector2D ClickedActorLocation2D(ClickedActorLocation);
				FVector NewActorLocation = BasePieceActor->GetActorLocation();
				FVector2D NewActorLocation2D(NewActorLocation);

				if (NewActorLocation == TileLocation) {
					ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
					GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
					GameMode->GField->TileMap[NewActorLocation2D]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
				}

				ClickCounter = 0;
				TurnCounter = TurnCounter + 1;
				//IsMyTurn = false;
			}
			//END OF ROOK TO EMPTY TILES MOVES SECTION
		}
		else if (ABasePiece* ClickedPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
				EPieceColor Color = ClickedPiece->GetPieceColor();

				if (Color == EPieceColor::BLACK) {
					FVector BlackActorLocation = ClickedPiece->GetActorLocation();

					// START PAWN TO OCCUPIED TILES MOVES SECTION
			        // Check if the first clicked actor was a pawn and if it's a legal move for the pawn
					if (BlackActorLocation.X - ClickedActorLocation.X == 120.0f && (BlackActorLocation.Y - ClickedActorLocation.Y == 120.0f || BlackActorLocation.Y - ClickedActorLocation.Y == -120.0f) && BasePieceActor->IsA(APawnChess::StaticClass())) {
						GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BLACK %s"), *BlackActorLocation.ToString()));
						ClickedPiece->Destroy();
						BasePieceActor->SetActorLocation(BlackActorLocation);
						ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
						FVector2D BlackActorLocation2D(BlackActorLocation);
						FVector2D ClickedActorLocation2D(ClickedActorLocation);
						FVector NewActorLocation = BasePieceActor->GetActorLocation();
						if (NewActorLocation == BlackActorLocation) {
							GameMode->GField->TileMap[BlackActorLocation2D]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
							GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
						}
						TurnCounter = TurnCounter + 1;
						ClickCounter = 0;
						//IsMyTurn = false;
					}
					// END PAWN TO OCCUPIED TILES MOVES SECTION

					//START ROOK TO OCCUPIED TILES MOVES SECTION
					if ((BlackActorLocation.X == ClickedActorLocation.X || BlackActorLocation.Y == ClickedActorLocation.Y) && BasePieceActor->IsA(ARook::StaticClass())) {
						GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("BLACK %s"), *BlackActorLocation.ToString()));
						ClickedPiece->Destroy();
						BasePieceActor->SetActorLocation(BlackActorLocation);
						ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
						FVector2D BlackActorLocation2D(BlackActorLocation);
						FVector2D ClickedActorLocation2D(ClickedActorLocation);
						FVector NewActorLocation = BasePieceActor->GetActorLocation();
						if (NewActorLocation == BlackActorLocation) {
							GameMode->GField->TileMap[BlackActorLocation2D]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
							GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
						}
						TurnCounter = TurnCounter + 1;
						ClickCounter = 0;
						//IsMyTurn = false;
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




