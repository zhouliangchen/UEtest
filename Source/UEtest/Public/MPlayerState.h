// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChangedSignature, float, NewCredits);
/**
 * 
 */
UCLASS()
class UETEST_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool UpdatePlayerCredits(float Delta);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChangedSignature OnCreditsChanged;
	AMPlayerState();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float Credits;
};
