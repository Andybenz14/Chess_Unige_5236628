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

	//Hit check and MyTurn check)
	if (Hit.bBlockingHit && IsMyTurn)
	{
		//Pawn actor hit check
		if (ABasePiece* CurrPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			BasePieceActor = CurrPiece;

			//Get hitted piece color
			EPieceColor Color = CurrPiece->GetPieceColor();

			//User can use only white pieces
			if (Color == EPieceColor::WHITE) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked"));
				ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
				ClickCounter = ClickCounter + 1;
				//Get clicked piece xyz location
				ClickedActorLocation = CurrPiece->GetActorLocation();
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Piece %s"), *ClickedActorLocation.ToString()));
			}
		}
	}

	if (Hit.bBlockingHit && IsMyTurn && ClickCounter != 0)
	{
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
		{
				//Get clicked piece xyz location
				FVector TileLocation = CurrTile->GetActorLocation();
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Piece %s"), *TileLocation.ToString()));
				BasePieceActor->SetActorLocation(TileLocation);
				FVector2D ClickedActorLocation2D(ClickedActorLocation);
				//Search Tile at the same position of the clicked piece and set owner and status
				ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
				GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::NONE, ETileStatus::EMPTY);
				
			}
		if (ABasePiece* ClickedPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			
			EPieceColor Color = ClickedPiece->GetPieceColor();
		
			if (Color == EPieceColor::BLACK) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked"));
				ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());
				//Get clicked piece xyz location
				ClickedActorLocation = ClickedPiece->GetActorLocation();
				GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Piece %s"), *ClickedActorLocation.ToString()));
				BasePieceActor->SetActorLocation(ClickedActorLocation);
				FVector2D ClickedActorLocation2D(ClickedActorLocation);
				GameMode->GField->TileMap[ClickedActorLocation2D]->SetTileStatus(ETileOwner::WHITE, ETileStatus::OCCUPIED);
			}
		}
		ClickCounter = 0;
		IsMyTurn = false;

	}
}
	



void ACHS_HumanPlayer::OnTurn()
{
	IsMyTurn = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your Turn"));
	
}




