// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MProjectileBase.h"
#include "MBlackholeProjectile.generated.h"


class URadialForceComponent;
/**
 * 
 */
UCLASS()
class UETEST_API AMBlackholeProjectile : public AMProjectileBase
{
	GENERATED_BODY()
public:
	AMBlackholeProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* RadialForceComp;
	virtual void BeginPlay() override;
};
