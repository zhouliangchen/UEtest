// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

//服务节点
//待读取变量：TargetKey目标actor, Distance 最大战斗距离
//待设置变量：AttackRangeKey 目标是否进入战斗范围 true为目标在范围外
//从blackboard获取目标，进而获取位置,再从AIController获取自身位置,计算距离,结合可见性判断,输出结果
void UMBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
		if(TargetActor)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			APawn* AIPawn = AIController->GetPawn();
			if(ensure(AIPawn))
			{
				float Distance = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
				if (Distance <= DistancetoAttack && AIController->LineOfSightTo(TargetActor))
				{
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, false);
				}
				else BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, true);
			}
		}
	}



}
