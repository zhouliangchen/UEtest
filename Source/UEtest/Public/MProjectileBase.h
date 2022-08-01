// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;


UCLASS(Abstract)
class UETEST_API AMProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMProjectileBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* ImpactVFX;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleComp;

	UFUNCTION()
	virtual void ProjectileHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1,
				   FVector Vector, const FHitResult& HitResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();
	//默认实现：在目标点生成粒子效果并析构自己
	virtual void Explode_Implementation();
	virtual void PostInitializeComponents() override;
public:	

};
