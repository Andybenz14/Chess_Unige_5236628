// Fill out your copyright notice in the Description page of Project Settings.


#include "Queen.h"
#include "CHS_GameMode.h"

// Sets default values
AQueen::AQueen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQueen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQueen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQueen::QueenPossibleMoves(FVector QueenLocation, ETileOwner Color)
{
	ACHS_GameMode* GameMode = (ACHS_GameMode*)(GetWorld()->GetAuthGameMode());

	PossibleQueenMoves.Empty();
	GameMode->Rook->RookPossibleMoves(QueenLocation, Color);
	GameMode->Bishop->BishopPossibleMoves(QueenLocation, Color);
	PossibleQueenMoves.Append(GameMode->Rook->PossibleRookMoves);
	PossibleQueenMoves.Append(GameMode->Bishop->PossibleBishopMoves);
}