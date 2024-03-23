// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameField.h"
#include "GameFramework/GameModeBase.h"
#include "CHS_GameMode.generated.h"

class AActor;

struct FPosition;

/**
 * 
 */
UCLASS()
class CHESS_UNIGE_5236628_API ACHS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGameField> GameFieldClass;

	// Field size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 FieldSize;

	// Turn counter
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 IsMyTurn;

	// Reference to a GameField object
	UPROPERTY(VisibleAnywhere)
		AGameField* GField;

	// End human turn
	void EndHumanTurn();

	// End AI turn
	void EndAITurn();

	// Bool game finished
	bool IsGameFinished() const;

	ACHS_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UCHS_GameInstance* GameInstance;

	
};
