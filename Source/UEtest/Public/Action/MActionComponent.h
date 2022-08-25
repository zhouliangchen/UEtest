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
	//���ڼ���� Tag
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
	//��Ϸ��ʼʱ��Ĭ�ϵĳ�ʼ����Action
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TArray<TSubclassOf<UMAction>> DefaultActions;
	//����Action
	UPROPERTY()
	TArray<UMAction*> Actions;
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//��������Ӧֹͣ����Action������Comp����ֹͣ����Action�������Խ����bug���������־��Action�������Ч��
	//�磬��Burn Effect�����󣬻����ȼ���StopAllAction���ټ���StopAction�������ظ�
	//���类�ӵ��������Իᴥ��Burn
	bool bAlive;
		
};
