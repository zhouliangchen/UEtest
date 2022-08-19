// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UETEST_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMPlayerState();
	
protected:
	UPROPERTY(EditAnywhere, Category = "GamePlay")
	float Credits;
	friend class UMGamePlayFunctionLibrary;
};
