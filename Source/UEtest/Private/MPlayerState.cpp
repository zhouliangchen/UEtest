// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

AMPlayerState::AMPlayerState():Credits(0.0f)
{
}

bool AMPlayerState::UpdatePlayerCredits(float Delta)
{
	if (Delta < 0.0f && Credits + Delta < 0.0f)
	{
		return false;
	}
	else
	{
		Credits += Delta;
		OnCreditsChanged.Broadcast(Credits);
		return true;
	}
	return false;
}
