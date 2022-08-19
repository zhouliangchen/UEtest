// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, AActor*, InstigatorActor, UMAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DELEGATE_TwoParams(FOnActorKilledSignature, AActor*, AActor*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UETEST_API UMAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMAttributeComponent();
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UMAttributeComponent* GetAttributeComp(AActor* Actor);
	//当目标死亡，目标不存在，或者目标没有MAttributeComponent时返回false
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(AActor* Actor);
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Attributes")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;

public:
	UPROPERTY(BlueprintAssignable,Category="Attributes")
	FOnHealthChangedSignature OnHealthChanged;
	FOnActorKilledSignature OnActorKilled;

	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive()const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth()const;
	float GetHealthMax()const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);
};
