// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPickupItemBase.h"
#include "MHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class UETEST_API AMHealthPotion : public AMPickupItemBase
{
	GENERATED_BODY()
public:
	AMHealthPotion();
protected:
	UPROPERTY(EditAnywhere)
	float PowerUpNumber;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float HealCost;
	virtual bool DoSthtoPawn_Implementation(APawn* InstigatorPawn) override;
};
