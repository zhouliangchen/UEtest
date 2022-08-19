// Fill out your copyright notice in the Description page of Project Settings.


#include "MHealthPotion.h"
#include "MAttributeComponent.h"
#include "MGamePlayFunctionLibrary.h"
#include "MPlayerController.h"
#include "UEtestGameModeBase.h"

AMHealthPotion::AMHealthPotion():PowerUpNumber(50.0f),HealCost(50.0f)
{
}

bool AMHealthPotion::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(InstigatorPawn);
	if (AttributeComp)
	{
		//为了尽量避免无谓的Credit变动，进行预先检查
		if (!AttributeComp->IsFullHealth() && AttributeComp->IsAlive() &&
			UMGamePlayFunctionLibrary::UpdatePlayerCredits(InstigatorPawn->GetController<AMPlayerController>(), -HealCost))
		{
			if (AttributeComp->ApplyHealthChange(this, PowerUpNumber))
			{
				return true;
			}
			else
			{
				//失败时退钱
				UMGamePlayFunctionLibrary::UpdatePlayerCredits(InstigatorPawn->GetController<AMPlayerController>(), HealCost);
				UE_LOG(LogTemp, Warning, TEXT("HealthPotion使用失败，已退还消耗Credit"));
			}
		}
	}
	return false;
}
