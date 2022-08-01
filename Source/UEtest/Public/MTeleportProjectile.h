// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MProjectileBase.h"
#include "MTeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UETEST_API AMTeleportProjectile : public AMProjectileBase
{
	GENERATED_BODY()
protected:
	FTimerHandle TeleportTimer;
	UPROPERTY(EditAnywhere, Category = "Teleport")
	float MoveTime;
	UPROPERTY(EditAnywhere, Category = "Teleport")
	float TeleportTime;
public:
	AMTeleportProjectile();
protected:
	virtual void Explode_Implementation() override;
	virtual void BeginPlay()override;
	void Teleport();
};
