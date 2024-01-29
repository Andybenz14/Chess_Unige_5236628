// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePiece.h"
#include "GameFramework/Actor.h"
#include "Bishop.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API ABishop : public ABasePiece
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABishop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
