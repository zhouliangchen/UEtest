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
	//标识当前Action是否正在运行
	UPROPERTY()
	bool bIsRunning;
	//一般为Action的来源/触发者
	UPROPERTY()
	AActor* Instigator;
	//由于同一来源的Effect会刷新持续时间，为了触发客户端UI更新，设bool变量
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
	//bEmergency 用于来自外部的Stop，跳过不必要的步骤
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator,bool bEmergency);
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	virtual UWorld* GetWorld() const override;
	bool IsRunning()const;
	//可用性。因为网络同步问题，在Server上的Action被Stop并Remove后，不能正常地在Client触发Stop，故延迟Remove，改为设此变量
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
