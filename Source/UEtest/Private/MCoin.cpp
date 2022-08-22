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
		return true;
	}
	return false;
}
