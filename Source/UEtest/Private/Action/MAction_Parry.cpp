// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MAction_Parry.h"

UMAction_Parry::UMAction_Parry():ParryingDuration(1.5f)
{
}

void UMAction_Parry::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	//Delegate.BindUFunction(this, STATIC_FUNCTION_FNAME(TEXT("UMAction::StopAction")), Instigator);
	Delegate.BindUObject(this, &UMAction::StopAction, Instigator);
	GetWorld()->GetTimerManager().SetTimer(ParryingTimer, Delegate, ParryingDuration, false);
}

