// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/MAction.h"
#include "MAction_Projectile.generated.h"

class AMProjectileBase;
/**
 * 
 */
UCLASS()
class UETEST_API UMAction_Projectile : public UMAction
{
	GENERATED_BODY()
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	void SpawnProjectile(ACharacter* Instigator);
	UMAction_Projectile();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AMProjectileBase> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UAnimMontage* AnimAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem* SpellVFX;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FName HandSocket;
	FTimerHandle AttackTimeHandle;
};
