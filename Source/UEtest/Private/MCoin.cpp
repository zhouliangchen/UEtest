// Fill out your copyright notice in the Description page of Project Settings.


#include "MCoin.h"
#include "MPlayerController.h"
#include "MGamePlayFunctionLibrary.h"

AMCoin::AMCoin():CoinReward(30.0f)
{
}

bool AMCoin::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	AMPlayerController* PlayerController = InstigatorPawn->GetController<AMPlayerController>();
	if (PlayerController && UMGamePlayFunctionLibrary::UpdatePlayerCredits(PlayerController, CoinReward))
	{
		return true;
	}
	return false;
}
