// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MAction.h"

#include "Action/MActionComponent.h"
#include "Net/UnrealNetwork.h"

UMAction::UMAction() :AutoStart(false)
{
}
void UMAction::StartAction_Implementation(AActor* Instigator)
{
	OwnerComp->ActiveGameplayTags.AppendTags(GrantTags);
	UE_LOG(LogTemp, Log, TEXT("Action %s Activated"), *GetNameSafe(this));
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void UMAction::StopAction_Implementation(AActor* Instigator, bool bEmergency)
{
	//ensureAlwaysMsgf(bIsRunning, TEXT("Action %s Stopped while it's not running!"), *GetNameSafe(this));
	OwnerComp->ActiveGameplayTags.RemoveTags(GrantTags);
	UE_LOG(LogTemp, Log, TEXT("Action %s Stopped"), *GetNameSafe(this));
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

bool UMAction::CanStart_Implementation(AActor* Instigator)
{
	//若不作IsRunning检查，则可在Stop前连续触发；
	//对于Projectile，若同类Action共用TimerHandle，会导致重复起手动作，但仅会触发一次Stop;若TimerHandle为局部变量，则回调互不影响，造成IsRunning在Stop阶段异常
	//若在BlockedTags中加入本Action，可替代IsRunning的作用
	return !IsRunning() && !OwnerComp->ActiveGameplayTags.HasAny(BlockedTags);
}

UWorld* UMAction::GetWorld() const
{
	AActor* Outer = Cast<AActor>(GetOuter());
	if(Outer)
	{
		return Outer->GetWorld();
	}
	return nullptr;
}

bool UMAction::IsRunning()const
{
	return RepData.bIsRunning;
}

void UMAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else StopAction(RepData.Instigator, false);
}

void UMAction::InitializeAction(UMActionComponent* Comp)
{
	OwnerComp = Comp;
}


UMActionComponent* UMAction::GetOwnerComp() const
{
	return OwnerComp;
}

void UMAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMAction, RepData);
	DOREPLIFETIME(UMAction, OwnerComp);
}