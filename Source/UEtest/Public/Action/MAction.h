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

	UPROPERTY()
	bool bIsRunning;
	UPROPERTY()
	AActor* Instigator;
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
	void OnRep_RepData();
	UPROPERTY(Replicated)
	UMActionComponent* OwnerComp;
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
	UMAction();
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
