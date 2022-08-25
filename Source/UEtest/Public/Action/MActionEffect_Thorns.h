// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/MActionEffect.h"
#include "MActionEffect_Thorns.generated.h"

class UMAttributeComponent;
/**
 * 
 */
UCLASS()
class UETEST_API UMActionEffect_Thorns : public UMActionEffect
{
	GENERATED_BODY()
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator, bool bEmergency) override;
	UMActionEffect_Thorns();
protected:
	UFUNCTION()
	void ReflectDamage(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta);
	//·´µ¯ÉËº¦µÄ±ÈÀý
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float ReflectRate;
};
