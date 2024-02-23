// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CHS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_UNIGE_5236628_API UCHS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString CurrentTurnMessage = "Current Player";

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetTurnMessage();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetTurnMessage(FString Message);

	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString WinMessage = "";

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetWinMessage();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetWinMessage(FString Message);

	// message to show every turn
	UPROPERTY(EditAnywhere)
		FString RegisterMove = "";

	// get the current turn message
	UFUNCTION(BlueprintCallable)
		FString GetRegisterMove();

	// set the turn message
	UFUNCTION(BlueprintCallable)
		void SetRegisterMove(FString Message);
	
	UFUNCTION(BlueprintCallable)
		bool GetNewText();

	UPROPERTY(EditAnywhere)
		bool NewText = false;
};
