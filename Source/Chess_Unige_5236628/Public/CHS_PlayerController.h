// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CHS_HumanPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CHS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_UNIGE_5236628_API ACHS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ACHS_PlayerController();

	UPROPERTY(EditAnywhere, Category = Input)
		UInputMappingContext* TTTContext;


	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* ClickAction;

	void ClickOnGrid();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
};
