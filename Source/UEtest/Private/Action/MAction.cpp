// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MAction.h"

#include "Action/MActionComponent.h"
#include "Action/MActionEffect.h"
#include "Net/UnrealNetwork.h"

UMAction::UMAction() :AutoStart(false),bActive(true)
{
}
void UMAction::StartAction_Implementation(AActor* Instigator)
{
	OwnerComp->ActiveGameplayTags.AppendTags(GrantTags);
	UE_LOG(LogTemp, Log, TEXT("Action %s Activated"), *GetNameSafe(this));
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
	if(OwnerComp->GetOwnerRole()==ROLE_Authority)
	{
		TimeStarted = GetWorld()->GetTimeSeconds();
	}
	OwnerComp->OnActionStarted.Broadcast(OwnerComp, this);
}

void UMAction::StopAction_Implementation(AActor* Instigator, bool bEmergency)
{
	//ensureAlwaysMsgf(bIsRunning, TEXT("Action %s Stopped while it's not running!"), *GetNameSafe(this));
	OwnerComp->ActiveGameplayTags.RemoveTags(GrantTags);
	UE_LOG(LogTemp, Log, TEXT("Action %s Stopped"), *GetNameSafe(this));
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
	OwnerComp->OnActionStopped.Broadcast(OwnerComp, this, WidgetInstance);
}

bool UMAction::CanStart_Implementation(AActor* Instigator)
{
	//������IsRunning��飬�����Stopǰ����������
	//����Projectile����ͬ��Action����TimerHandle���ᵼ���ظ����ֶ����������ᴥ��һ��Stop;��TimerHandleΪ�ֲ���������ص�����Ӱ�죬���IsRunning��Stop�׶��쳣
	//����BlockedTags�м��뱾Action�������IsRunning������
	return bActive && !IsRunning() && !OwnerComp->ActiveGameplayTags.HasAny(BlockedTags);
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

bool UMAction::IsActive() const
{
	return bActive;
}

void UMAction::OnRep_RepData(FActionRepData OldRepData)
{
	if (RepData.bIsRunning)
	{
		if(OldRepData.bIsRunning)
		{
			Cast<UMActionEffect>(this)->ReStartEffect(RepData.Instigator);
		}
		else
		{
			StartAction(RepData.Instigator);
		}
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
	DOREPLIFETIME(UMAction, TimeStarted);
	DOREPLIFETIME(UMAction, bActive);
	DOREPLIFETIME(UMAction, OwnerComp);
}