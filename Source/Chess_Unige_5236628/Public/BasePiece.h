// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePiece.generated.h"


UENUM()
enum class EPieceColor : uint8
{
	WHITE     UMETA(DisplayName = "White"),
	BLACK     UMETA(DisplayName = "BLack"),

};

UCLASS()
class CHESS_UNIGE_5236628_API ABasePiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePiece();

	// Set the (x, y) position
	void SetBasePieceGridPosition(const double InX, const double InY);

	// Get the (x, y) position
	FVector2D GetBasePieceGridPosition();

	// White material interface 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface* WhiteMaterial;

	// Black material interface 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
		UMaterialInterface* BlackMaterial;

	// Set tile material
	void SetBasePieceMaterial(const int32 ElementIndex, UMaterialInterface* TileMaterial);


	// Set the color of the piece
	void SetPieceColor(const EPieceColor Color);

	// Get piece color
	EPieceColor GetPieceColor();

	// Destroy a base sign actor
	UFUNCTION()
		void SelfDestroy();


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Scene
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* Scene;

	// Static mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector2D BasePieceGridPosition;

	// Color of the piece
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EPieceColor Colour;


};
