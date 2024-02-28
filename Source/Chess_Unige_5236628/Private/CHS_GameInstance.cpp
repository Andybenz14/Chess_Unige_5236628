// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_GameInstance.h"
#include <iostream>
#include <string>
#include <sstream>

FString UCHS_GameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}

void UCHS_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

FString UCHS_GameInstance::GetWinMessage()
{
	return WinMessage;
}

void UCHS_GameInstance::SetWinMessage(FString Message)
{
	WinMessage = Message;
}

FString UCHS_GameInstance::GetRegisterMove()
{
	return RegisterMove;
}

void UCHS_GameInstance::SetRegisterMove(FString Message)
{
	NewText = true;
	RegisterMove = Message;
	NewText = false;
	Moves.Add(Message);
}

bool UCHS_GameInstance::GetNewText()
{
	return NewText;
}

TArray<FString> UCHS_GameInstance::GetMovesArray() const
{

	return Moves;
}

void UCHS_GameInstance::Replay(FString SelectedMove)
{
	std::string InputMove = TCHAR_TO_UTF8(*SelectedMove);
	std::istringstream Token(InputMove);
	std::string Color;
	int Number;

	Token >> Color >> Number;

	if (Color == "(Black)") 
	{
		Number = Number + 1;
	}

	FString NumberString = FString::Printf(TEXT("Il valore di Number è: %d"), Number);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, NumberString);

}
