// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsChangedSignature, float, NewCredits);
/**
 * 
 */
UCLASS()
class UETEST_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnCreditsChangedSignature OnCreditsChanged;
};
