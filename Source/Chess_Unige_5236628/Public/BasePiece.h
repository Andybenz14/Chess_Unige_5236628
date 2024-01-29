// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePiece.generated.h"


UENUM()
enum class EPieceColor : uint8
{
	WHITE     UMETA(DisplayName = "White"),
	BLACK      UMETA(DisplayName = "Black"),
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
		EPieceColor Color;

//public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	// destroy a base sign actor
	//UFUNCTION()
	//void SelfDestroy();

};
