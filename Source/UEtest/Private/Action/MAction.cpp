// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MAction.h"

void UMAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action %s Activated"), *GetNameSafe(this));
}

void UMAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action %s Stopped"), *GetNameSafe(this));
}

UWorld* UMAction::GetWorld() const
{
	UActorComponent* Outer = Cast<UActorComponent>(GetOuter());
	if(Outer)
	{
		return Outer->GetWorld();
	}
	return nullptr;
}
