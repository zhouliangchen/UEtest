// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MBTTask_Heal.h"

#include "AIController.h"
#include "MAttributeComponent.h"
#include "AI/MAICharacter.h"

EBTNodeResult::Type UMBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if(!AIController)
	{
		return EBTNodeResult::Aborted;
	}
	AMAICharacter* AICharacter = Cast<AMAICharacter>(AIController->GetPawn());
	if(!AICharacter)
	{
		return EBTNodeResult::Failed;
	}
	UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(AICharacter);
	bool ret = false;
	if(AttributeComp)
	{
		ret = AttributeComp->ApplyHealthChange(AICharacter, AttributeComp->GetHealthMax());
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Cyan, ret ? TEXT("AI治疗成功") : TEXT("AI治疗失败"));
	}
	return ret ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
