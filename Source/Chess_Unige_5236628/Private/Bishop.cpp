// Fill out your copyright notice in the Description page of Project Settings.


#include "Bishop.h"

// Sets default values
ABishop::ABishop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BasePieceGridPosition = FVector2D(0, 0);

}

// Called when the game starts or when spawned
void ABishop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABishop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


