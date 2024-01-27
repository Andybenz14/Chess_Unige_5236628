// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CHS_PlayerInterface.h"
#include "CHS_HumanPlayer.generated.h"

UCLASS()
class CHESS_UNIGE_5236628_API ACHS_HumanPlayer : public APawn, public ICHS_PlayerInterface

{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACHS_HumanPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
