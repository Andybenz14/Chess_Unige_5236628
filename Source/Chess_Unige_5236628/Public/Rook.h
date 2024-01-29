// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSign.h"
#include "GameFramework/Actor.h"
#include "Rook.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API ARook : public ABaseSign
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARook();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
