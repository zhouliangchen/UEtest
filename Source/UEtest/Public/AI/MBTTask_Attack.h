// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MProjectileBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MBTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class UETEST_API UMBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UMBTTask_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AMProjectileBase> Projectile;
	UPROPERTY(EditAnywhere, Category = "AI")
	float BulletSpread;
};
