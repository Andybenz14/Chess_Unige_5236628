// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	Status = ETileStatus::EMPTY;
	PlayerOwner = ETileOwner::NONE;
	TileGridPosition = FVector2D(0, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATile::SetTileStatus(const ETileOwner TileOwner, const ETileStatus TileStatus)
{
	PlayerOwner = TileOwner;
	Status = TileStatus;
}

// Apply TileMaterial to the StaticMeshComponent of the tile
void ATile::SetTileMaterial(const int32 ElementIndex, UMaterialInterface* TileMaterial)
{
	StaticMeshComponent->SetMaterial(0, TileMaterial);
}

UMaterialInterface* ATile::GetTileMaterial(const int32 ElementIndex) const
{
	if (StaticMeshComponent)
	{
		return StaticMeshComponent->GetMaterial(ElementIndex);
	}
	return nullptr;
}

ETileStatus ATile::GetTileStatus()
{
	return Status;
}

ETileOwner ATile::GetOwner()
{
	return PlayerOwner;
}

void ATile::SetGridPosition(const double InX, const double InY)
{
	TileGridPosition.Set(InX, InY);
}

FVector2D ATile::GetGridPosition()
{
	return TileGridPosition;
}

// Called every frame
//void ATile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);

//}

