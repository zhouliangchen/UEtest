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
	bool bIsRunning;
public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool AutoStart;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	//bEmergency 用于来自外部的Stop，跳过不必要的步骤
	void StopAction(AActor* Instigator,bool bEmergency);
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	virtual UWorld* GetWorld() const override;
	bool IsRunning()const;
	UMAction();
};
