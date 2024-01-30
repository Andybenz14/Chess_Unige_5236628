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

UENUM()
enum class EPieceStartingPosition : uint8
{
	B1     UMETA(DisplayName = "B1 White Knight"),
	G1     UMETA(DisplayName = "G1 White Knight"),
	B8     UMETA(DisplayName = "B8 Black Knight"),
	G8     UMETA(DisplayName = "G8 Black Knight"),

	C1     UMETA(DisplayName = "C1 White Bishop"),
	F1     UMETA(DisplayName = "F1 White Bishop"),
	C8     UMETA(DisplayName = "C8 Black Bishop"),
	F8     UMETA(DisplayName = "F8 Black Bishop"),

	A1     UMETA(DisplayName = "A1 White Rook"),
	H1     UMETA(DisplayName = "H1 White Rook"),
	A8     UMETA(DisplayName = "A8 Black Rook"),
	H8     UMETA(DisplayName = "H8 Black Rook"),

	D1     UMETA(DisplayName = "D1 White Queen"),
	D8     UMETA(DisplayName = "D8 White Queen"),

	E1     UMETA(DisplayName = "E1 White King"),
	E8     UMETA(DisplayName = "E8 White King"),

	A2     UMETA(DisplayName = "A2 White Pawn"),
	B2     UMETA(DisplayName = "B2 White Pawn"),
	C2     UMETA(DisplayName = "C2 White Pawn"),
	D2     UMETA(DisplayName = "D2 White Pawn"),
	E2     UMETA(DisplayName = "E2 White Pawn"),
	F2     UMETA(DisplayName = "F2 White Pawn"),
	G2     UMETA(DisplayName = "G2 White Pawn"),
	H2     UMETA(DisplayName = "H2 White Pawn"),
	A7     UMETA(DisplayName = "A8 Black Pawn"),
	B7     UMETA(DisplayName = "B8 Black Pawn"),
	C7     UMETA(DisplayName = "C1 Black Pawn"),
	D7     UMETA(DisplayName = "D8 Black Pawn"),
	E7     UMETA(DisplayName = "E8 Black Pawn"),
	F7     UMETA(DisplayName = "F1 Black Pawn"),
	G7     UMETA(DisplayName = "G8 Black Pawn"),
	H7     UMETA(DisplayName = "H8 Black Pawn"),


};


UCLASS()
class CHESS_UNIGE_5236628_API ABasePiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePiece();

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

	// Pieces starting position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
		EPieceStartingPosition StartPosition;

//public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	// destroy a base sign actor
	//UFUNCTION()
	//void SelfDestroy();

};
