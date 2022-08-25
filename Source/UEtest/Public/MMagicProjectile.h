// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MProjectileBase.h"
#include "NativeGameplayTags.h"
#include "MMagicProjectile.generated.h"

class UMActionEffect;
UCLASS()
class UETEST_API AMMagicProjectile : public AMProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMMagicProjectile();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	FGameplayTag ParryTag;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UMActionEffect> BurningActionEffect;
	UFUNCTION()
	void DealDamage(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int Index, bool bArg, const FHitResult& HitResult);
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float Damage;
	virtual void PostInitializeComponents() override;
	bool bReflected;
};
