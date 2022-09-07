// Fill out your copyright notice in the Description page of Project Settings.


#include "MCoin.h"
#include "MPlayerController.h"
#include "MGamePlayFunctionLibrary.h"
#include "MPlayerState.h"

AMCoin::AMCoin():CoinReward(30.0f)
{
}

bool AMCoin::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	AMPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AMPlayerState>();
	if(PlayerState&&PlayerState->UpdatePlayerCredits(CoinReward))
	{
		//UE_LOG(LogTemp, Log, TEXT("GetName: %s"), *(this->GetName()));
		//UE_LOG(LogTemp, Log, TEXT("GetFName: %s"), *this->GetFName().ToString());
		//UE_LOG(LogTemp, Log, TEXT("GetFullName: %s"), *(this->GetFullName()));
		//UE_LOG(LogTemp, Log, TEXT("GetPathName: %s"), *(this->GetPathName()));
		//UE_LOG(LogTemp, Log, TEXT("GetActorNameOrLabel: %s"), *(this->GetActorNameOrLabel()));
		return true;
	}
	return false;
}
