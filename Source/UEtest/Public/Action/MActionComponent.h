// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MActionComponent.generated.h"

class UMAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UETEST_API UMActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="Action")
	bool AddAction(TSubclassOf<UMAction> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);
	// Sets default values for this component's properties
	UMActionComponent();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TArray<TSubclassOf<UMAction>> DefaultActions;

	UPROPERTY()
	TArray<UMAction*> Actions;
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
