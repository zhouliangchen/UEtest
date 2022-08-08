// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

//����ڵ�
//����ȡ������TargetKeyĿ��actor, Distance ���ս������
//�����ñ�����AttackRangeKey Ŀ���Ƿ����ս����Χ trueΪĿ���ڷ�Χ��
//��blackboard��ȡĿ�꣬������ȡλ��,�ٴ�AIController��ȡ����λ��,�������,��Ͽɼ����ж�,������
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
