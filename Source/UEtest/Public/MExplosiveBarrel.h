// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class UETEST_API AMExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AMExplosiveBarrel();


protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComp;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1,
	               FVector Vector, const FHitResult& HitResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
