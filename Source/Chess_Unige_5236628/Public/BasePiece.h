// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePiece.generated.h"

// Settable pieces' color (white or black)
UENUM()
enum class EPieceColor : uint8
{
	WHITE     UMETA(DisplayName = "White"),
	BLACK     UMETA(DisplayName = "Black"),
};


UCLASS()
class CHESS_UNIGE_5236628_API ABasePiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePiece();

	// set the (x, y) position
	void SetBasePieceGridPosition(const double InX, const double InY);

	// get the (x, y) position
	FVector2D GetBasePieceGridPosition();

	// White material interface 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface* WhiteMaterial;

	// Black material interface 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface* BlackMaterial;

	// set tile material
	void SetBasePieceMaterial(const int32 ElementIndex, UMaterialInterface* TileMaterial);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Scene
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* Scene;

	// Static mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;

	// Pieces color 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
		EPieceColor Color;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector2D BasePieceGridPosition;


};
