// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "MActionComponent.generated.h"

class UMActionEffect;
class UMAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UETEST_API UMActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//正在激活的 Tag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer ActiveGameplayTags;
		
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool AddAction(TSubclassOf<UMAction> ActionClass, AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool AddActionUnique(TSubclassOf<UMAction> ActionClass, AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool AddActionEffect(TSubclassOf<UMActionEffect> ActionClass, AActor* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool RemoveAction(UMAction* Action);
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);
	UFUNCTION(BlueprintCallable, Category = "Action")
	void StopAllAction();
	UFUNCTION(BlueprintCallable, Category = "Components")
	static UMActionComponent* GetActionComp(AActor* Actor);
	// Sets default values for this component's properties
	UMActionComponent();
	
protected:
	//游戏开始时，默认的初始可用Action
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TArray<TSubclassOf<UMAction>> DefaultActions;
	//可用Action
	UPROPERTY()
	TArray<UMAction*> Actions;
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//在死亡后应停止所有Action，但由Comp主动停止所有Action会有难以解决的bug，所以设标志让Action来检测有效性
	//如，被Burn Effect烧死后，会首先激活StopAllAction，再继续StopAction，产生重复
	//再如被子弹打死后，仍会触发Burn
	bool bAlive;
		
};
