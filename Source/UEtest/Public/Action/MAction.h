// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "MAction.generated.h"

class UMActionComponent;
/**
 * 
 */
USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
public:
	//��ʶ��ǰAction�Ƿ���������
	UPROPERTY()
	bool bIsRunning;
	//һ��ΪAction����Դ/������
	UPROPERTY()
	AActor* Instigator;
	//����ͬһ��Դ��Effect��ˢ�³���ʱ�䣬Ϊ�˴����ͻ���UI���£���bool����
	UPROPERTY()
	bool ChangeBit;
};


UCLASS(Blueprintable)
class UETEST_API UMAction : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagContainer GrantTags;
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagContainer BlockedTags;
	UFUNCTION(BlueprintCallable, Category = "Action")
	UMActionComponent* GetOwnerComp() const;
	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;
	UFUNCTION()
	void OnRep_RepData(FActionRepData OldRepData);
	UPROPERTY(Replicated)
	UMActionComponent* OwnerComp;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	UTexture2D* Icon;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget* WidgetInstance;
	UPROPERTY(Replicated)
	float TimeStarted;
	
public:
	void InitializeAction(UMActionComponent* Comp);
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool AutoStart;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	//bEmergency ���������ⲿ��Stop����������Ҫ�Ĳ���
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator,bool bEmergency);
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	virtual UWorld* GetWorld() const override;
	bool IsRunning()const;
	//�����ԡ���Ϊ����ͬ�����⣬��Server�ϵ�Action��Stop��Remove�󣬲�����������Client����Stop�����ӳ�Remove����Ϊ��˱���
	UPROPERTY(Replicated)
	bool bActive;
	bool IsActive()const;
	const FGameplayTagContainer& GetBlockedTags()const
	{
		return BlockedTags;
	}
	UMAction();
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	
};
