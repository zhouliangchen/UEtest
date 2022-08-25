// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/MAction.h"
#include "MActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class UETEST_API UMActionEffect : public UMAction
{
	GENERATED_BODY()
public:
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bEmergency) override;
	virtual void ReStartEffect(AActor* Instigator);
	UMActionEffect();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;
	UPROPERTY(BlueprintReadOnly, Category="Effect")
	AActor* EffectInstigator;
	FTimerHandle DurationTimer;
	FTimerHandle PeriodTimer;
	FTimerDelegate DurationDelegate;
	FTimerDelegate PeriodDelegate;
	virtual void PreExecuteEffect();
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecuteEffect();
};
