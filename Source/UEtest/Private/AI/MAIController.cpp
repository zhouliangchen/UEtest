// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AMAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	// APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if(PlayerPawn)
	// {
	// 	//GetBlackboardComponent()->SetValueAsVector("MovetoLocation", PlayerPawn->GetActorLocation());
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	// }
}
