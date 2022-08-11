// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MBTTask_Attack.h"

#include "AIController.h"
#include "AI/MAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


//Task任务：对TargetKey目标Actor开火，即Spawn Projectile
EBTNodeResult::Type UMBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Aborted;
	}
	AMAICharacter* AICharactor = Cast<AMAICharacter>(Controller->GetPawn());
	if (!AICharactor)
	{
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	if(!TargetActor)
	{
		return EBTNodeResult::Failed;
	}
	FVector SpawnLocation = AICharactor->GetMesh()->GetSocketLocation(AICharactor->HandSocket);
	FVector TargetLocation = TargetActor->GetActorLocation();
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();
	FActorSpawnParameters Param;
	Param.Instigator = AICharactor;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(Projectile, SpawnLocation, SpawnRotation, Param);
	return SpawnActor ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
