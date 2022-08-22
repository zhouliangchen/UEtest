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
	//������IsRunning��飬�����Stopǰ����������
	//����Projectile����ͬ��Action����TimerHandle���ᵼ���ظ����ֶ����������ᴥ��һ��Stop;��TimerHandleΪ�ֲ���������ص�����Ӱ�죬���IsRunning��Stop�׶��쳣
	//����BlockedTags�м��뱾Action�������IsRunning������
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
