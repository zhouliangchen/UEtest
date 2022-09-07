// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChangedSignature, AActor*, InstigatorActor, UMAttributeComponent*, OwningComp, float, NewValue, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeReplicatedSignature, UMAttributeComponent*, OwningComp, float, NewValue);
DECLARE_DELEGATE_TwoParams(FOnActorKilledSignature, AActor*, AActor*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UETEST_API UMAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMAttributeComponent();
	UFUNCTION(BlueprintCallable, Category = "Components")
	static UMAttributeComponent* GetAttributeComp(AActor* Actor);
	//��Ŀ��������Ŀ�겻���ڣ�����Ŀ��û��MAttributeComponentʱ����false
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(AActor* Actor);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing="OnRep_Health", Category = "Attributes")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing="OnRep_Energy", Category = "Attributes")
	float Energy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Attributes")
	float EnergyMax;
	//�ܵ��˺�ת��Ϊ�����ı�ֵ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float DamageConvertionRate;
	//����˺�ת��Ϊ�����ı�ֵ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float AttackConvertionRate;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastEnergyChanged(AActor* InstigatorActor, float NewEnergy, float Delta);
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttributeChangedSignature OnEnergyChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttributeReplicatedSignature OnHealthReplicated;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttributeReplicatedSignature OnEnergyReplicated;
	FOnActorKilledSignature OnActorKilled;
	UFUNCTION()
	void OnRep_Health();
	UFUNCTION()
	void OnRep_Energy();
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	//Energy ������ƣ�ͨ���˺����ˡ����˺�����ɱ���ˡ����߻�ȡ���ͷż�������  Instigator�������Ǻ�����չ
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyEnergyChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsEnergyEnough(float EnergyNeeded)const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive()const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth()const;
	float GetHealthMax()const;
	void SetHealthBeforeReplicate(float NewHealth);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);
};
