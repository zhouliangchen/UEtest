// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MActionComponent.h"

#include "EngineUtils.h"
#include "Action/MAction.h"
#include "Action/MActionEffect.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "UEtest/UEtest.h"


// Sets default values for this component's properties
UMActionComponent::UMActionComponent():bAlive(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}



// Called when the game starts
void UMActionComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UMAction> ActionClass : DefaultActions)
		{
			AddAction(ActionClass, GetOwner());
		}
	}
}


// Called every frame
void UMActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple());
	for (UMAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

bool UMActionComponent::AddAction(TSubclassOf<UMAction> ActionClass, AActor* Instigator)
{
	if(!ensureMsgf(ActionClass,TEXT("ActionClass未设置")))
	{
		return false;
	}
	if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction %s"), *GetNameSafe(ActionClass));
	}
	if (ActionClass->IsChildOf(UMActionEffect::StaticClass()))
	{
		for (UMAction* ExistAction : Actions)
		{
			if (ExistAction->IsA(ActionClass))
			{
				//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, TEXT("检测到已有ActionEffect重复添加: ") + GetNameSafe(ExistAction));
				Cast<UMActionEffect>(ExistAction)->ReStartEffect(Instigator);
				return true;
			}
		}
	}
	UMAction* NewAction = NewObject<UMAction>(this, ActionClass);
	if(!ensureMsgf(NewAction,TEXT("NewAction生成失败")))
	{
		return false;
	}
	NewAction->InitializeAction(this);
	Actions.Add(NewAction);
	if (NewAction->AutoStart && NewAction->CanStart(Instigator))
	{
		NewAction->StartAction(Instigator);
	}
	return true;
}

bool UMActionComponent::AddActionUnique(TSubclassOf<UMAction> ActionClass, AActor* Instigator)
{
	if (!ensureMsgf(ActionClass, TEXT("ActionClass未设置")))
	{
		return false;
	}
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction %s"), *GetNameSafe(ActionClass));
	}
	for (UMAction* ExistAction : Actions)
	{
		if (ExistAction->IsA(ActionClass))
		{
			return false;
		}
	}
	UMAction* NewAction = NewObject<UMAction>(this, ActionClass);
	if (!ensureMsgf(NewAction, TEXT("NewAction生成失败")))
	{
		return false;
	}
	NewAction->InitializeAction(this);
	Actions.Add(NewAction);
	if (NewAction->AutoStart && NewAction->CanStart(Instigator))
	{
		NewAction->StartAction(Instigator);
	}
	return true;
}

bool UMActionComponent::RemoveAction(UMAction* Action)
{
	//需要先Stop再Remove
	if(Action&&!Action->IsRunning())
	{
		Actions.RemoveSingleSwap(Action);
		return true;
	}
	return false;
}

bool UMActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(UMAction* Action: Actions)
	{
		if (Action->ActionName == ActionName && Action->CanStart(Instigator))
		{
			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

void UMActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	for (UMAction* Action : Actions)
	{
		if (Action->ActionName == ActionName && Action->CanStart(Instigator))
		{
			Action->StartAction(Instigator);
			return;
		}
	}
}

bool UMActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(UMAction* Action:Actions)
	{
		//特别针对release按键触发StopAction的情形，做IsRunning检查
		if (Action->ActionName == ActionName && Action->IsRunning())
		{
			if(!GetOwner()->HasAuthority())
			{
				ServerStopAction(Instigator, ActionName);
			}
			Action->StopAction(Instigator, false);
			return true;
		}
	}
	return false;
}

void UMActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	for (UMAction* Action : Actions)
	{
		//特别针对release按键触发StopAction的情形，做IsRunning检查
		if (Action->ActionName == ActionName && Action->IsRunning())
		{
			Action->StopAction(Instigator, false);
			return;
		}
	}
}

bool UMActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething=Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for(UMAction* Action:Actions)
	{
		WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
	}
	return WroteSomething;
}

void UMActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMActionComponent, Actions);
}

void UMActionComponent::StopAllAction()
{
	for(UMAction* Action:Actions)
	{
		if(Action->IsRunning())
		{
			Action->StopAction(GetOwner(), true);
		}
	}
}

UMActionComponent* UMActionComponent::GetActionComp(AActor* Actor)
{
	if(Actor)
	{
		return Cast<UMActionComponent>(Actor->GetComponentByClass(UMActionComponent::StaticClass()));
	}
	return nullptr;
}
