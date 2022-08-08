// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class UETEST_API UMBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;
	UPROPERTY(EditAnywhere, Category = "AI")
	float DistancetoAttack;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
