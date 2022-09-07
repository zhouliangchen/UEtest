// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MActionEffect.h"

#include <tuple>

#include "MAttributeComponent.h"
#include "Action/MActionComponent.h"
#include "GameFramework/GameStateBase.h"


UMActionEffect::UMActionEffect():Duration(3.0f),Period(1.0f)
{
	AutoStart = true;
}


void UMActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (OwnerComp->GetOwner()->HasAuthority())
	{
		EffectInstigator = Instigator;
		if (Duration > 0.0f)
		{
			DurationDelegate.BindUObject(this, &UMActionEffect::StopAction, Instigator, false);
			GetWorld()->GetTimerManager().SetTimer(DurationTimer, DurationDelegate, Duration, false);
		}
		if (Period > 0.0f)
		{
			PeriodDelegate.BindUObject(this, &UMActionEffect::PreExecuteEffect);
			GetWorld()->GetTimerManager().SetTimer(PeriodTimer, PeriodDelegate, Period, true);
		}
	}
}

void UMActionEffect::StopAction_Implementation(AActor* Instigator, bool bEmergency)
{
	if (!bEmergency && GetWorld()->GetTimerManager().GetTimerRemaining(PeriodTimer) < KINDA_SMALL_NUMBER)
	{
		PreExecuteEffect();
	}
	Super::StopAction_Implementation(Instigator, bEmergency);
	GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
	GetWorld()->GetTimerManager().ClearTimer(PeriodTimer);
	bActive = false;
	WidgetInstance = nullptr;
	//此处必须在StopAllAction时跳过，否则会产生迭代器失效bug
	// if (!bEmergency && OwnerComp)
	// {
	// 	OwnerComp->RemoveAction(this);
	// }
}

void UMActionEffect::ReStartEffect(AActor* Instigator)
{
	if(OwnerComp->GetOwnerRole()==ROLE_Authority)
	{
		TimeStarted = GetWorld()->GetTimeSeconds();
	}
	EffectInstigator = Instigator;
	if (Duration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DurationTimer, DurationDelegate, Duration, false);
		UE_LOG(LogTemp, Log, TEXT("Action %s Reactivated"), *GetNameSafe(this));
		if(OwnerComp->GetOwner()->HasAuthority())
		{
			RepData.ChangeBit = !RepData.ChangeBit;
		}
	}
}

float UMActionEffect::GetRemainingTime() const
{
	if(Duration)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		if(GameState)
		{
			float RemainingTime = Duration + TimeStarted - GameState->GetServerWorldTimeSeconds();
			return RemainingTime > 0 ? RemainingTime : 0;
		}
		return Duration;
	}
	return BIG_NUMBER;
}

void UMActionEffect::PreExecuteEffect()
{
	if(OwnerComp->bActive)
	{
		ExecuteEffect();
	}
}

void UMActionEffect::ExecuteEffect_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Effect %s Execute, instigator: %s"), *GetNameSafe(this), *GetNameSafe(EffectInstigator));
}

