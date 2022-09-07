// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MProjectileBase.generated.h"

class USoundCue;
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;

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
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundCue* ImpactSound;
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<UCameraShakeBase> Shake;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComp;

	UFUNCTION()
	virtual void ProjectileHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1,
				   FVector Vector, const FHitResult& HitResult);
	//默认实现：在目标点生成粒子效果并析构自己
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();
	//原子变量，起锁的作用，防止多次调用explode（必须在类声明并初始化，否则报错）(多人模式Client会由本地和RPC调用，只响应其中任一，以保证视觉的流畅与体验的稳定）
	std::atomic_flag bActive = ATOMIC_FLAG_INIT;
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastExplode();
	virtual void PostInitializeComponents() override;
public:	

};
