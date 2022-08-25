// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPickupItemBase.h"
#include "MActionPoint.generated.h"

class UMAction;
/**
 * 
 */
UCLASS()
class UETEST_API AMActionPoint : public AMPickupItemBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TSubclassOf<UMAction> ActionClass;;
	virtual bool DoSthtoPawn_Implementation(APawn* InstigatorPawn) override;
};
