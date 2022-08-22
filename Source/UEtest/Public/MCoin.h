// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPickupItemBase.h"
#include "MCoin.generated.h"

/**
 * 
 */
UCLASS()
class UETEST_API AMCoin : public AMPickupItemBase
{
	GENERATED_BODY()
public:
	AMCoin();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float CoinReward;
	virtual bool DoSthtoPawn_Implementation(APawn* InstigatorPawn) override;
};
