// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/MAction.h"
#include "MAction_Parry.generated.h"

/**
 * 
 */
UCLASS()
class UETEST_API UMAction_Parry : public UMAction
{
	GENERATED_BODY()
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	UMAction_Parry();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	float ParryingDuration;
	FTimerHandle ParryingTimer;
	FTimerDelegate Delegate;
};
