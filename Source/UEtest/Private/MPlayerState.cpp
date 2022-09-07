// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

#include <string>

#include "MSaveGame.h"
#include "Net/UnrealNetwork.h"
#include "UEtest/UEtest.h"

AMPlayerState::AMPlayerState():Credits(0.0f)
{
	bReplicates = true;
}

void AMPlayerState::OnRep_Credits()
{
	UE_LOG(LogTemp, Log, TEXT("%s %s"), GetWorld()->IsNetMode(NM_Client) ? TEXT("[CLIENT] ") : TEXT("[SERVER] "), *FString::SanitizeFloat(Credits));
	OnCreditsReplicated.Broadcast(Credits);
}

bool AMPlayerState::UpdatePlayerCredits(float Delta)
{
	if (Delta < 0.0f && Credits + Delta < 0.0f)
	{
		return false;
	}
	else
	{
		if(HasAuthority())
		{
			Credits += Delta;
			OnRep_Credits();
		}
		//UE_LOG(LogTemp, Log, TEXT("%s %s"), GetWorld()->IsNetMode(NM_Client) ? TEXT("[CLIENT] ") : TEXT("[SERVER] "), *FString::SanitizeFloat(Credits));
		return true;
	}
	return false;
}

void AMPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMPlayerState, Credits);
}

void AMPlayerState::LoadPlayerState_Implementation(UMSaveGame* SaveGame)
{
	if(SaveGame)
	{
		Credits = SaveGame->Credits;
		OnRep_Credits();//在加载后必须触发UI更新
	}
}

void AMPlayerState::SavePlayerState_Implementation(UMSaveGame* SaveGame)
{
	if(SaveGame)
	{
		SaveGame->Credits = Credits;
	}
}
