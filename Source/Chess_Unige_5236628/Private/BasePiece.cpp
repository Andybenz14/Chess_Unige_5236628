// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePiece.h"
#include "CHS_GameMode.h"
#include "GameField.h"


// Sets default values
ABasePiece::ABasePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	BasePieceGridPosition = FVector2D(0, 0);

}

// Called when the game starts or when spawned
void ABasePiece::BeginPlay()
{
	Super::BeginPlay();

	

	
}

//Set grid position
void ABasePiece::SetBasePieceGridPosition(const double InX, const double InY)
{
	BasePieceGridPosition.Set(InX, InY);
}

//Get grid position
FVector2D ABasePiece::GetBasePieceGridPosition()
{
	return BasePieceGridPosition;
}

//Set piece material
void ABasePiece::SetBasePieceMaterial(const int32 ElementIndex, UMaterialInterface* TileMaterial)
{
	StaticMeshComponent->SetMaterial(0, TileMaterial);
}

//Actor destroy
void ABasePiece::SelfDestroy()
{
	Destroy();
}

// Apply TileMaterial to the StaticMeshComponent of the tile
void ABasePiece::SetPieceColor(const EPieceColor Color)
{
	Colour = Color;
}

//Get piece color
EPieceColor ABasePiece::GetPieceColor()
{
	return Colour;
}