// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MActionComponent.h"

#include "Action/MAction.h"


// Sets default values for this component's properties
UMActionComponent::UMActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for(TSubclassOf<UMAction> ActionClass :DefaultActions)
	{
		AddAction(ActionClass);
	}
}


// Called every frame
void UMActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UMActionComponent::AddAction(TSubclassOf<UMAction> ActionClass)
{
	if(!ensureMsgf(ActionClass,TEXT("ActionClass未设置")))
	{
		return false;
	}
	UMAction* NewAction = NewObject<UMAction>(this, ActionClass);
	if(!ensureMsgf(NewAction,TEXT("NewAction生成失败")))
	{
		return false;
	}
	Actions.AddUnique(NewAction);
	return true;
}

bool UMActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(UMAction* Action: Actions)
	{
		if(Action->ActionName==ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UMActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(UMAction* Action:Actions)
	{
		if(Action->ActionName==ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}
