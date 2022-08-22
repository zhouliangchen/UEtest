// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MAction.h"

#include "Action/MActionComponent.h"

void UMAction::StartAction_Implementation(AActor* Instigator)
{
	GetOwnerComp()->ActiveGameplayTags.AppendTags(GrantTags);
	UE_LOG(LogTemp, Log, TEXT("Action %s Activated"), *GetNameSafe(this));
	bIsRunning = true;
}

void UMAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlwaysMsgf(bIsRunning, TEXT("Action %s Stopped while it's not running!"), *GetNameSafe(this));
	GetOwnerComp()->ActiveGameplayTags.RemoveTags(GrantTags);
	UE_LOG(LogTemp, Log, TEXT("Action %s Stopped"), *GetNameSafe(this));
	bIsRunning = false;
}

bool UMAction::CanStart_Implementation(AActor* Instigator)
{
	//若不作IsRunning检查，则可在Stop前连续触发；
	//对于Projectile，若同类Action共用TimerHandle，会导致重复起手动作，但仅会触发一次Stop;若TimerHandle为局部变量，则回调互不影响，造成IsRunning在Stop阶段异常
	//若在BlockedTags中加入本Action，可替代IsRunning的作用
	return !IsRunning() && !GetOwnerComp()->ActiveGameplayTags.HasAny(BlockedTags);
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

bool UMAction::IsRunning()const
{
	return bIsRunning;
}

UMActionComponent* UMAction::GetOwnerComp() const
{
	return Cast<UMActionComponent>(GetOuter());
}
