// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeComponent.h"


// Sets default values for this component's properties
UMAttributeComponent::UMAttributeComponent():Health(100.0f),HealthMax(100.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// // Called every frame
// void UMAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// 	GEngine->AddOnScreenDebugMessage(1, 1.0, FColor::Yellow, FString::SanitizeFloat(Health));
// }

//��Ѫ���䶯ʱ����true��������OnHealthChanged�¼�
bool UMAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	Delta = NewHealth - Health;
	Health = NewHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
	//���ҪΪÿ��Actor�����˺�������Ӧ���������
	return Delta ? true : false;
}

bool UMAttributeComponent::IsAlive()const
{
	return Health > 0.0f;
}

bool UMAttributeComponent::IsFullHealth() const
{
	//Ŀǰ��û�г���С��λ�ϵı䶯������ֱ����==�Ƚϸ�����
	return Health == HealthMax;
}

float UMAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

