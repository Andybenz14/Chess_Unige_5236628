// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM()
enum class ETileStatus : uint8
{
	EMPTY     UMETA(DisplayName = "Empty"),
	OCCUPIED      UMETA(DisplayName = "Occupied"),
};

UENUM()
enum class ETileOwner : uint8
{
	WHITE     UMETA(DisplayName = "White"),
	BLACK     UMETA(DisplayName = "BLack"),
	NONE      UMETA(DisplayName = "None"),
};



UCLASS()
class CHESS_UNIGE_5236628_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// White material interface 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface* WhiteMaterial;

	// Black material interface 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface* BlackMaterial;

	// set tile material
	void SetTileMaterial(const int32 ElementIndex, UMaterialInterface* TileMaterial);

	// set the player owner and the status of a tile
	void SetTileStatus(const ETileOwner TileOwner, const ETileStatus TileStatus);

	// get the tile status
	ETileStatus GetTileStatus();

	// get the tile owner
	ETileOwner GetOwner();

	// set the (x, y) position
	void SetGridPosition(const double InX, const double InY);

	// get the (x, y) position
	FVector2D GetGridPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ETileStatus Status;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ETileOwner PlayerOwner;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector2D TileGridPosition;

};
