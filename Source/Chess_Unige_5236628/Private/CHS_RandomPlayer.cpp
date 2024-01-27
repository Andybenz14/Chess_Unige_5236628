// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_RandomPlayer.h"

// Sets default values
ACHS_RandomPlayer::ACHS_RandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACHS_RandomPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACHS_RandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACHS_RandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

