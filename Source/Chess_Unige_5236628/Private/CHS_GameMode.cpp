// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_GameMode.h"
#include "CHS_HumanPlayer.h"
#include "CHS_PlayerController.h"
#include "GameField.h"
#include "EngineUtils.h"

ACHS_GameMode::ACHS_GameMode()
{
	PlayerControllerClass = ACHS_PlayerController::StaticClass();
	DefaultPawnClass = ACHS_HumanPlayer::StaticClass();
	FieldSize = 8;
}


void ACHS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	ACHS_HumanPlayer* HumanPlayer = Cast<ACHS_HumanPlayer>(*TActorIterator<ACHS_HumanPlayer>(GetWorld()));


	if (GameFieldClass != nullptr)
	{
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		GField->Size = FieldSize;

	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

}