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