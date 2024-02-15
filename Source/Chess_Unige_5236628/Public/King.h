// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "King.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API AKing : public ABasePiece
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		TArray<FVector2D> PossibleKingMoves;

	void KingPossibleMoves(FVector KingLocation, ETileOwner Color);

};
