// Fill out your copyright notice in the Description page of Project Settings.


#include "MHealthPotion.h"

#include "MAttributeComponent.h"
#include "MCharacter.h"
#include "MGamePlayFunctionLibrary.h"
#include "MPlayerController.h"
#include "MPlayerState.h"
#include "UEtestGameModeBase.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

AMHealthPotion::AMHealthPotion():PowerUpNumber(50.0f),HealCost(50.0f)
{
}

void AMHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn, FText& Message, FText& AdditionalMessage)
{
	AMCharacter* User = Cast<AMCharacter>(InstigatorPawn);
	if (!User)return;
	const UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(User);
	if (!AttributeComp)return;
	const AMPlayerState* PlayerState = User->GetPlayerState<AMPlayerState>();
	if(!PlayerState)return;
	if (AttributeComp->IsFullHealth())
	{
		Message = LOCTEXT("HealthPotion_FullHealth", "Already Full Health");
		return;
	}
	if(PlayerState->GetPlayerCredits()<HealCost)
	{
		Message = LOCTEXT("HealthPotion_NoCredits_0", "No Enough Credits");
		const FTextFormat HealthPotion_NoCredits = LOCTEXT("HealthPotion_NoCredits_1", "Cost {0} Credit to Use the Potion");
		AdditionalMessage = FText::FormatOrdered(HealthPotion_NoCredits, HealCost);
		return;
	}
	Message = LOCTEXT("HealthPotion_Interact", "'E' to Heal Yourself");
}

bool AMHealthPotion::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(InstigatorPawn);
	if (AttributeComp)
	{
		AMPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AMPlayerState>();
		ensureMsgf(PlayerState, TEXT("Interact检测不到PlayerState"));
		//为了尽量避免无谓的Credit变动，进行预先检查
		if (!AttributeComp->IsFullHealth() && AttributeComp->IsAlive() &&PlayerState->UpdatePlayerCredits(-HealCost))
		{
			if (AttributeComp->ApplyHealthChange(this, PowerUpNumber))
			{
				return true;
			}
			else
			{
				//失败时退钱
				PlayerState->UpdatePlayerCredits(HealCost);
				UE_LOG(LogTemp, Warning, TEXT("HealthPotion使用失败，已退还消耗Credit"));
			}
		}
	}
	return false;
}

#undef LOCTEXT_NAMESPACE