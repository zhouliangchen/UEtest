// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MProjectileBase.h"
#include "MMagicProjectile.generated.h"

UCLASS()
class UETEST_API AMMagicProjectile : public AMProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
