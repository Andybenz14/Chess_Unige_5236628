// Fill out your copyright notice in the Description page of Project Settings.


#include "CHS_GameInstance.h"

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
